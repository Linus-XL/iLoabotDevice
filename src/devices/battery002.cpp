#include "iloabot/devices/battery002.h"

ILoabotBattery002::ILoabotBattery002(std::string model)
    : ILoabotBattery(std::move(model)) {}

std::string ILoabotBattery002::name() const {
    return "iLoabot Battery 002 [" + model_ + "]";
}

int ILoabotBattery002::batteryLevel() const {
    return 200;
}
