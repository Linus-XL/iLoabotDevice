#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>

class Socket {
public:
    Socket();
    ~Socket();

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;
    Socket(Socket&& other) noexcept;
    Socket& operator=(Socket&& other) noexcept;

    bool create();
    void close();
    bool isValid() const;

    bool bind(const std::string& address, uint16_t port);
    bool listen(int backlog = 5);
    Socket accept();

    bool connect(const std::string& address, uint16_t port);

    int send(const uint8_t* data, std::size_t len);
    int receive(uint8_t* buffer, std::size_t maxLen);

    bool setReuseAddr(bool enable);
    bool setNonBlocking(bool enable);
    bool setTimeout(int timeoutMs);

private:
    struct Impl;

    explicit Socket(std::unique_ptr<Impl> impl);

    std::unique_ptr<Impl> impl_;
};
