#include "Server.hpp"

#include <iostream>
#include <sstream>
#include <mutex>
#include <cstring>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "OrderBook.hpp"
#include "Order.hpp"

namespace
{
    std::mutex ob_mutex;

    Type parseType(const std::string& s)
    {
        return (s == "BUY") ? BUY : SELL;
    }

    Order parseOrder(const std::string& msg)
    {
        std::stringstream ss(msg);

        std::string typeStr;
        double price;
        int qty;

        std::getline(ss, typeStr, ',');
        ss >> price;
        ss.ignore();
        ss >> qty;

        return Order(parseType(typeStr), qty, price);
    }
}

Server::Server(int port)
    : port_(port), server_fd_(-1), running_(false)
{
}

void Server::start()
{
    running_ = true;
    server_thread_ = std::thread(&Server::run, this);
}

void Server::stop()
{
    running_ = false;

    // connect to self to unblock accept()
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port_);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    connect(sock, (sockaddr*)&addr, sizeof(addr));
    close(sock);

    if (server_thread_.joinable())
        server_thread_.join();
}

void Server::run()
{
    OrderBook orderbook;

    server_fd_ = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_port = htons(port_);
    address.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd_, (sockaddr*)&address, sizeof(address));
    listen(server_fd_, 10);

    while (running_)
    {
        int client_fd = accept(server_fd_, nullptr, nullptr);

        if (client_fd < 0)
            continue;

        std::thread(&Server::handleClient, this, client_fd).detach();
    }

    close(server_fd_);
}

void Server::handleClient(int client_fd)
{
    static OrderBook orderbook; // shared across threads

    char buffer[1024] = {0};

    ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);

    if (bytes_read <= 0)
    {
        close(client_fd);
        return;
    }

    std::string msg(buffer);

    try
    {
        Order order = parseOrder(msg);

        {
            std::lock_guard<std::mutex> lock(ob_mutex);
            orderbook.addOrderAndMatch(order);
        }

        std::string response = "OK\n";
        send(client_fd, response.c_str(), response.size(), 0);
    }
    catch (...)
    {
        std::string response = "ERROR\n";
        send(client_fd, response.c_str(), response.size(), 0);
    }

    close(client_fd);
}
