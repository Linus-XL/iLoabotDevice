#pragma once

#include "iloabot/comm/connection.h"
#include "iloabot/comm/socket.h"

#include <cstdint>
#include <string>

struct TcpClientConfig {
    std::string address = "127.0.0.1";
    uint16_t    port    = 8080;
    int         timeoutMs = 3000;
};

class TcpClient : public Connection {
public:
    explicit TcpClient(const TcpClientConfig& config = {});
    ~TcpClient() override;

    TcpClient(const TcpClient&) = delete;
    TcpClient& operator=(const TcpClient&) = delete;

    bool open() override;
    void close() override;
    bool isOpen() const override;

    int send(const uint8_t* data, std::size_t len) override;
    int receive(uint8_t* buffer, std::size_t maxLen) override;

    void setConfig(const TcpClientConfig& config);
    const TcpClientConfig& config() const;

private:
    TcpClientConfig config_;
    Socket socket_;
};
