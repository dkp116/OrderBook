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

int main(int argc, char* argv[])
{
    bool run_server = false;
    int port = 8080;

    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--server") {
            run_server = true;
        } else if (arg == "--port" && i + 1 < argc) {
            port = std::stoi(argv[++i]);
        } else if (arg == "--help" || arg == "-h") {
            std::cout << "OrderBook Trading System\n"
                      << "Usage: " << argv[0] << " [options]\n"
                      << "Options:\n"
                      << "  --server          Run as TCP server\n"
                      << "  --port <number>   Server port (default: 8080)\n"
                      << "  --help, -h        Show this help message\n";
            return 0;
        }
    }

    if (run_server) {
        Server server(port);
        server.start();

        std::cout << "OrderBook TCP Server running on port " << port << "...\n"
                  << "Send orders in format: TYPE,PRICE,QUANTITY\n"
                  << "Example: BUY,100.50,50\n"
                  << "Press Enter to stop...\n";

        std::cin.get(); // keep alive

        std::cout << "Stopping server...\n";
        server.stop();
        std::cout << "Server stopped.\n";
    } else {
        std::cout << "OrderBook Trading System\n"
                  << "Run with --server to start TCP server\n"
                  << "Use --help for more options\n";
    }

    return 0;
}