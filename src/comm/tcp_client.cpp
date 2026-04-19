#include "iloabot/comm/tcp_client.h"

TcpClient::TcpClient(const TcpClientConfig& config) : config_(config) {}

TcpClient::~TcpClient() { close(); }

bool TcpClient::open() {
    close();
    if (!socket_.create()) return false;
    socket_.setTimeout(config_.timeoutMs);
    if (!socket_.connect(config_.address, config_.port)) {
        socket_.close();
        return false;
    }
    return true;
}

void TcpClient::close() {
    socket_.close();
}

bool TcpClient::isOpen() const {
    return socket_.isValid();
}

int TcpClient::send(const uint8_t* data, std::size_t len) {
    return socket_.send(data, len);
}

int TcpClient::receive(uint8_t* buffer, std::size_t maxLen) {
    return socket_.receive(buffer, maxLen);
}

void TcpClient::setConfig(const TcpClientConfig& config) {
    config_ = config;
}

const TcpClientConfig& TcpClient::config() const { return config_; }
