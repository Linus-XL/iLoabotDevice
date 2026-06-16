#pragma once

#include "iloabot/comm/connection.h"

#include <memory>
#include <string>

enum class BaudRate {
    Baud9600   = 9600,
    Baud19200  = 19200,
    Baud38400  = 38400,
    Baud57600  = 57600,
    Baud115200 = 115200
};

enum class DataBits { Five = 5, Six = 6, Seven = 7, Eight = 8 };
enum class Parity   { None, Odd, Even };
enum class StopBits { One, Two };

struct SerialConfig {
    std::string  device   = "/dev/ttyUSB0";
    BaudRate     baudRate = BaudRate::Baud115200;
    DataBits     dataBits = DataBits::Eight;
    Parity       parity   = Parity::None;
    StopBits     stopBits = StopBits::One;
    int          timeoutMs = 1000;
};

class SerialPort : public Connection {
public:
    explicit SerialPort(const SerialConfig& config = {});
    ~SerialPort() override;

    SerialPort(const SerialPort&) = delete;
    SerialPort& operator=(const SerialPort&) = delete;

    bool open() override;
    void close() override;
    bool isOpen() const override;

    int send(const uint8_t* data, std::size_t len) override;
    int receive(uint8_t* buffer, std::size_t maxLen) override;

    void setConfig(const SerialConfig& config);
    const SerialConfig& config() const;

private:
    struct Impl;

    bool applyConfig();
    SerialConfig config_;
    std::unique_ptr<Impl> impl_;
};
