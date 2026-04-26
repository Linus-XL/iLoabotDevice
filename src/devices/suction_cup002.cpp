#include "iloabot/devices/suction_cup002.h"

ILoabotSuctionCup002::ILoabotSuctionCup002(std::string model)
    : ILoabotSuctionCup(std::move(model)) {}

std::string ILoabotSuctionCup002::name() const {
    return "iLoabot SuctionCup 002 [" + model_ + "]";
}

bool ILoabotSuctionCup002::initialize() {
    status_ = DeviceStatus::Idle;
    on_ = false;
    return true;
}

void ILoabotSuctionCup002::turnOn() {
    on_ = true;
    status_ = DeviceStatus::Running;
}