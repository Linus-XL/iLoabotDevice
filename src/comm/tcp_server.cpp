#include "iloabot/comm/tcp_server.h"

TcpServer::TcpServer(const TcpServerConfig& config) : config_(config) {}

TcpServer::~TcpServer() { stop(); }

bool TcpServer::start() {
    stop();
    if (!listener_.create()) return false;
    listener_.setReuseAddr(true);

    if (config_.timeoutMs > 0) {
        listener_.setTimeout(config_.timeoutMs);
    }

    if (!listener_.bind(config_.bindAddress, config_.port)) {
        listener_.close();
        return false;
    }

    if (!listener_.listen(config_.backlog)) {
        listener_.close();
        return false;
    }

    running_ = true;
    return true;
}

void TcpServer::stop() {
    running_ = false;
    listener_.close();
}

bool TcpServer::isRunning() const {
    return running_ && listener_.isValid();
}

Socket TcpServer::acceptClient() {
    if (!isRunning()) return Socket();
    return listener_.accept();
}

void TcpServer::setConfig(const TcpServerConfig& config) {
    config_ = config;
}

const TcpServerConfig& TcpServer::config() const { return config_; }
