#pragma once

#include <atomic>
#include <thread>

class Server
{
public:
    explicit Server(int port = 8080);

    void start();
    void stop();

private:
    void run();
    void handleClient(int client_fd);

private:
    int port_;
    int server_fd_;

    std::atomic<bool> running_;
    std::thread server_thread_;
};