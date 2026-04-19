#include "iloabot/devices/battery.h"

std::string ILoabotBattery::name() const { return "iLoabot Battery"; }
std::string ILoabotBattery::type() const { return "Battery"; }

DeviceStatus ILoabotBattery::status() const { return status_; }

int ILoabotBattery::batteryLevel() const { return level_; }
bool ILoabotBattery::isCharging() const { return charging_; }
