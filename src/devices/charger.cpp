#include "iloabot/devices/charger.h"

std::string ILoabotCharger::name() const { return "iLoabot Charger"; }
std::string ILoabotCharger::type() const { return "Charger"; }

bool ILoabotCharger::initialize() {
    status_ = DeviceStatus::Idle;
    on_ = false;
    return true;
}

void ILoabotCharger::shutdown() {
    on_ = false;
    status_ = DeviceStatus::Offline;
}

void ILoabotCharger::reset() {
    on_ = false;
    status_ = DeviceStatus::Idle;
}

DeviceStatus ILoabotCharger::status() const { return status_; }

void ILoabotCharger::turnOn() {
    on_ = true;
    status_ = DeviceStatus::Running;
}

void ILoabotCharger::turnOff() {
    on_ = false;
    status_ = DeviceStatus::Idle;
}

bool ILoabotCharger::isOn() const { return on_; }
