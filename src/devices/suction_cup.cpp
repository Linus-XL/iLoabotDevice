#include "iloabot/devices/suction_cup.h"

std::string ILoabotSuctionCup::name() const { return "iLoabot SuctionCup"; }
std::string ILoabotSuctionCup::type() const { return "SuctionCup"; }

bool ILoabotSuctionCup::initialize() {
    status_ = DeviceStatus::Idle;
    on_ = false;
    return true;
}

void ILoabotSuctionCup::shutdown() {
    on_ = false;
    status_ = DeviceStatus::Offline;
}

void ILoabotSuctionCup::reset() {
    on_ = false;
    status_ = DeviceStatus::Idle;
}

DeviceStatus ILoabotSuctionCup::status() const { return status_; }

void ILoabotSuctionCup::turnOn() {
    on_ = true;
    status_ = DeviceStatus::Running;
}

void ILoabotSuctionCup::turnOff() {
    on_ = false;
    status_ = DeviceStatus::Idle;
}

bool ILoabotSuctionCup::isOn() const { return on_; }
