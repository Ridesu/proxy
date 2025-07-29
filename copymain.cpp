#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket) : socket_(std::move(socket)) {}

    void start() {
        do_read();
    }

private:
    tcp::socket socket_;
    enum { max_length = 20000 };
    std::vector<char> data_;


    std::string fix_connection_header(std::string request) {
        std::string connection_key = "Connection:";
        std::size_t pos = request.find(connection_key);

        if (pos != std::string::npos) {
            // Знайшли старий заголовок — замінюємо рядок повністю
            std::size_t end_line = request.find("\r\n", pos);
            if (end_line != std::string::npos) {
                request.replace(pos, end_line - pos, "Connection: close");
            }
        } else {
            // Не знайшли — вставляємо перед кінцем заголовків
            std::size_t headers_end = request.find("\r\n\r\n");
            if (headers_end != std::string::npos) {
                request.insert(headers_end, "Connection: close\r\n");
            }
        }

        return request;
    }

    std::string GetAbsoluteDommen(std::string line)
    {
        std::string http_prefix = "http://";
        
        auto first_space = line.find(" ");
        if (first_space == std::string::npos) return "";

        auto second_space = line.find(" ", first_space + 1);
        if (second_space == std::string::npos) return "";

        std::string url = line.substr(first_space + 1, second_space - first_space - 1);

        if (url.compare(0, http_prefix.size(), http_prefix) == 0) {
            // Відрізаємо "http://"
            std::string rest = url.substr(http_prefix.size());

            // Знайдемо перший '/' після домену
            size_t path_pos = rest.find('/');
            if (path_pos != std::string::npos) {
                return rest.substr(path_pos); // повертаємо шлях разом з '/'
            } else {
                // Якщо '/' немає, значить запит root "/"
                return "/";
            }
        }

        // Якщо URL вже відносний (починається з '/'), повертаємо як є
        if (!url.empty() && url[0] == '/') {
            return url;
        }

        // Якщо формат інший — повертаємо порожнє
        return "";
    }

    std::string SendRequestToSite(std::string request, std::string host)
    {
        boost::asio::io_context ioContext;
        request = fix_connection_header(request);
        tcp::resolver resolver{ioContext};
        auto endpoints = resolver.resolve(host, "http");
        tcp::socket remote_socket(ioContext);
        boost::asio::connect(remote_socket, endpoints);

        boost::asio::write(remote_socket, boost::asio::buffer(request));

        boost::system::error_code error;
        std::ostringstream response_data;
        std::array<char, 4096> buffer;

        // Читаємо, поки є дані
        for (;;) {
            size_t bytes_read = remote_socket.read_some(boost::asio::buffer(buffer), error);
            if (error == boost::asio::error::eof) {
                // Кінець даних — нормально
                break;
            }
            else if (error) {
                throw boost::system::system_error(error);
            }
            response_data.write(buffer.data(), bytes_read);
        }

        return response_data.str();

    }

    void do_read() {
        auto self(shared_from_this());
        data_.resize(8000);
        socket_.async_read_some(boost::asio::buffer(data_),
            [this, self](boost::system::error_code ec, std::size_t length) {
                if (!ec && length > 0) {
                    std::string full_response(data_.data(), length);
                    std::cout <<"a" << std::endl << full_response << std::endl;
                    
                    int position = full_response.find("Host:") + 6;
                    int end_of_line = full_response.find("\r\n", position);

                    std::string host = full_response.substr(position, end_of_line-position);
                    std::string first_line = full_response.substr(0, full_response.find("\n\r"));
                    
                    
                    std::string response_data = SendRequestToSite(full_response, host);

                    data_.assign(response_data.begin(), response_data.end());

                    do_write(data_.size());
                }
                else if (ec == boost::asio::error::eof) {
                // Клієнт закрив з'єднання, більше даних не буде
                std::cout << "З'єднання закрите клієнтом\n";
            }
            });
    }

    void do_write(std::size_t length) {
        auto self(shared_from_this());
        boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
            [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                if (!ec) {
                    do_read();
                }
            });
    }
};


class ProxyServer {
public:
    ProxyServer(boost::asio::io_context& io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
        start_accept();
    }

private:
    tcp::acceptor acceptor_;

    void start_accept() {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket) {
                if (!ec) {
                    std::make_shared<Session>(std::move(socket))->start();
                }
                start_accept();
            });
    }
};
int main() {
    try {
        boost::asio::io_context io_context;
        ProxyServer server(io_context, 5555);
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
}