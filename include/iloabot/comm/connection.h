#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

class Connection {
public:
    virtual ~Connection() = default;

    virtual bool open() = 0;
    virtual void close() = 0;
    virtual bool isOpen() const = 0;

    virtual int send(const uint8_t* data, std::size_t len) = 0;
    virtual int receive(uint8_t* buffer, std::size_t maxLen) = 0;

    int send(const std::string& data) {
        return send(reinterpret_cast<const uint8_t*>(data.data()), data.size());
    }

    std::string receiveString(std::size_t maxLen = 4096) {
        std::vector<uint8_t> buf(maxLen);
        int n = receive(buf.data(), maxLen);
        if (n > 0) {
            return std::string(buf.begin(), buf.begin() + n);
        }
        return {};
    }
};
