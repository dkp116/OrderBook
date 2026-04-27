#include <iostream>
#include <string>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "OrderBook.hpp"
#include "Order.hpp"

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

int main()
{
    
    OrderBook orderbook;

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    address.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (sockaddr*)&address, sizeof(address));
    listen(server_fd, 5);

    std::cout << "Order server running on port 8080...\n";

    while (true)
    {
        int client = accept(server_fd, nullptr, nullptr);

        char buffer[1024] = {0};
        read(client, buffer, 1024);

        std::string msg(buffer);

        Order order = parseOrder(msg);

        orderbook.addOrderAndMatch(order);

        close(client);
    }
}