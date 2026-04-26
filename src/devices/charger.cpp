#include "iloabot/devices/charger.h"

ILoabotCharger::ILoabotCharger(std::string model) : model_(std::move(model)) {}

std::string ILoabotCharger::type() const { return "Charger"; }
std::string ILoabotCharger::model() const { return model_; }

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
