#include "iloabot/devices/suction_cup002.h"
#include "iloabot/comm/serial_port.h"

#include <cctype>
#include <sstream>
#include <iostream>
#include <vector>

namespace
{

    bool parseHexCommand(const std::string &command, std::vector<uint8_t> &out)
    {
        out.clear();
        std::istringstream iss(command);
        std::string token;
        while (iss >> token)
        {
            if (token.size() > 2)
            {
                return false;
            }
            for (unsigned char ch : token)
            {
                if (!std::isxdigit(ch))
                {
                    return false;
                }
            }
            unsigned int value = 0;
            std::stringstream converter;
            converter << std::hex << token;
            converter >> value;
            if (value > 0xFF)
            {
                return false;
            }
            out.push_back(static_cast<uint8_t>(value));
        }

        return !out.empty();
    }

} // namespace

ILoabotSuctionCup002::ILoabotSuctionCup002(std::string model)
    : ILoabotSuctionCup(std::move(model))
{
    serialPort_.setConfig({.device = "COM5",
                           .baudRate = BaudRate::Baud115200,
                           .dataBits = DataBits::Eight,
                           .parity = Parity::None,
                           .stopBits = StopBits::One,
                           .timeoutMs = 1000});
}

std::string ILoabotSuctionCup002::name() const
{
    return "iLoabot SuctionCup 002 [" + model_ + "]";
}

bool ILoabotSuctionCup002::initialize()
{
    if (serialPort_.open())
    {
        status_ = DeviceStatus::Idle;
        on_ = true;
        return true;
    }
    return false;
}

void ILoabotSuctionCup002::turnOn()
{
    on_ = true;
    status_ = DeviceStatus::Running;
    std::cout << name() << " is turned ON.\n";
}

void ILoabotSuctionCup002::sendCommand(const std::string &command)
{
    if (!isOn())
    {
        std::cerr << "Cannot send command. " << name() << " is not ON.\n";
        return;
    }

    std::vector<uint8_t> bytes;
    const bool useHexFrame = command.find(' ') != std::string::npos;
    const uint8_t *payload = nullptr;
    std::size_t payloadLen = 0;

    if (useHexFrame)
    {
        if (!parseHexCommand(command, bytes))
        {
            std::cerr << "Invalid hex command format for " << name() << ": " << command << '\n';
            return;
        }
        payload = bytes.data();
        payloadLen = bytes.size();
    }
    else
    {
        payload = reinterpret_cast<const uint8_t *>(command.data());
        payloadLen = command.size();
    }

    const int result = serialPort_.send(payload, payloadLen);
    if (result < 0)
    {
        std::cerr << "Failed to send command to " << name() << ". Serial write returned " << result << '\n';
        return;
    }

    std::cout << "Sent " << result << " bytes to " << name() << ": " << command << std::endl;
}
void ILoabotSuctionCup002::receivemsg(std::string &msg)
{
    std::vector<uint8_t> receiveBuffer(1024); // 预分配缓冲区大小
    size_t received = serialPort_.receive(receiveBuffer.data(), receiveBuffer.size());
    receiveBuffer.resize(received); // 调整为实际接收的长度
    std::cout << "Received " << received << " bytes from " << name() << ": ";
    for (uint8_t temp:receiveBuffer)
    {
       std::cout << std::hex << static_cast<int>(temp) << " ";
    }
    std::cout << std::dec << std::endl; // 切换回十进制输出
    
    // 直接赋值，避免临时对象
    msg.assign(reinterpret_cast<const char *>(receiveBuffer.data()), receiveBuffer.size());
}