#pragma once

#include "iloabot/comm/socket.h"

#include <cstdint>
#include <functional>
#include <string>

struct TcpServerConfig {
    std::string bindAddress = "0.0.0.0";
    uint16_t    port        = 8080;
    int         backlog     = 5;
    int         timeoutMs   = 0;
};

class TcpServer {
public:
    using ClientHandler = std::function<void(Socket client)>;

    explicit TcpServer(const TcpServerConfig& config = {});
    ~TcpServer();

    TcpServer(const TcpServer&) = delete;
    TcpServer& operator=(const TcpServer&) = delete;

    bool start();
    void stop();
    bool isRunning() const;

    Socket acceptClient();

    void setConfig(const TcpServerConfig& config);
    const TcpServerConfig& config() const;

private:
    TcpServerConfig config_;
    Socket listener_;
    bool running_ = false;
};
