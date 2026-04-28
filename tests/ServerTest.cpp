#include <gtest/gtest.h>
#include <thread>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <sstream>
#include <streambuf>
#include "Server.hpp"

std::string send_order(const std::string& msg)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr));

    send(sock, msg.c_str(), msg.size(), 0);

    char buffer[1024] = {0};
    read(sock, buffer, sizeof(buffer));

    close(sock);

    return std::string(buffer);
}

TEST(IntegrationTest, BasicOrderAccepted)
{
    Server server(8080);
    server.start();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::string response = send_order("BUY,100,10\n");

    EXPECT_EQ(response, "OK\n");

    server.stop();
}




TEST(IntegrationTest, MatchAppearsInLogs)
{
    testing::internal::CaptureStdout();

    Server server(8080);
    server.start();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    send_order("BUY,100,10\n");
    send_order("SELL,100,10\n");

    server.stop();

    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("MATCH"), std::string::npos);
}