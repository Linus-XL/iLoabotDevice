#pragma once

#include "iloabot/devices/suction_cup.h"
#include "iloabot/comm/serial_port.h"

class ILoabotSuctionCup002 final : public ILoabotSuctionCup
{
public:
    explicit ILoabotSuctionCup002(std::string model = "SC-200");

    std::string name() const override;
    bool initialize() override;
    void turnOn() override;
    void sendCommand(const std::string &command) override;
    void receivemsg(std::string &msg);

private:
    SerialPort serialPort_;
};