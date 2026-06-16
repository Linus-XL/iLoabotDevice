#include "iloabot/devices/charger002.h"

ILoabotCharger002::ILoabotCharger002(std::string model)
    : ILoabotCharger(std::move(model)) {}

std::string ILoabotCharger002::name() const {
    return "iLoabot Charger 002 [" + model_ + "]";
}

void ILoabotCharger002::turnOn() {
    on_ = true;
    status_ = DeviceStatus::Running;
}
