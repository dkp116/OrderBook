#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <mutex>
#include <vector>
#include <cstring>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "OrderBook.hpp"
#include "Order.hpp"

#include "Server.hpp"

int main()
{
    Server server(8080);
    server.start();

    std::cout << "Server running...\n";
    std::cin.get(); // keep alive

    server.stop();
    return 0;
}