# HTTP Proxy Server (C++)

This project is a **simple HTTP proxy server written in C++**, created for learning how proxy servers work, how HTTP traffic flows, and how network traffic can be intercepted and analyzed.

The proxy allows a client (browser) to **manually select the proxy server IP address**, after which all HTTP traffic passes through the proxy and can be inspected.

> ⚠️ This project is intended for **educational and laboratory use only**.  
> It supports **HTTP only** and is not designed for production environments.

---

## 🧠 Project Overview

This proxy acts as an intermediary between a browser and a destination server.

Key characteristics:
- The **client manually configures the proxy IP address in the browser**
- The proxy works **only with HTTP traffic**
- All requests and responses pass through the proxy, allowing **traffic inspection**
- Designed to demonstrate how proxies intercept and forward traffic

This project focuses on **networking fundamentals and traffic analysis**, not encryption or anonymity.

---

## 🌐 How the Proxy Is Used

1. The proxy server is started on a specific IP address and port
2. The client configures their **browser proxy settings** to point to that IP
3. The browser sends HTTP requests through the proxy
4. The proxy forwards requests to the destination server
5. Responses are received and sent back to the client
6. HTTP traffic can be observed or logged inside the proxy

---

## 📂 Repository Structure

proxy/<br>
├── copymain.cpp # Main C++ proxy server implementation<br>
├── server1 # Auxiliary server / example files<br>
├── README.md<br>


---

## 🛠️ Technologies Used

- **C++**
- **TCP sockets**
- **HTTP protocol**
- Low-level network programming

---

## 🚀 Build & Run

### Compile

```bash
g++ copymain.cpp -o proxy
```

### Run the Proxy

```bash
./proxy
```

## 🌍 Browser Configuration

# To use the proxy, configure your browser settings:

Proxy type: Manual

HTTP proxy: "<proxy_ip>:<proxy_port>"

HTTPS proxy: disabled (not supported)

Once configured, all HTTP traffic from the browser will pass through the proxy.

## 🔍 Learning Objectives

# This project helps to understand:

How HTTP proxy servers work

How browsers interact with proxies

Manual proxy configuration in clients

Traffic interception and inspection

Low-level socket programming in C++

These concepts are fundamental for network engineering, debugging, and cybersecurity analysis.

## ⚖️ Ethical Use & Disclaimer

This proxy is intended for learning and experimentation in controlled environments.
Intercepting network traffic without permission is unethical and illegal.

# Use this project only on systems and networks you own or are authorized to test.

## 👤 Author

### Ridesu
Junior Developer / Cybersecurity Learner
