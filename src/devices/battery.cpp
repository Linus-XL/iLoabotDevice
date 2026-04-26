#include "iloabot/devices/battery.h"

ILoabotBattery::ILoabotBattery(std::string model) : model_(std::move(model)) {}

std::string ILoabotBattery::type() const { return "Battery"; }
std::string ILoabotBattery::model() const { return model_; }

DeviceStatus ILoabotBattery::status() const { return status_; }

int ILoabotBattery::batteryLevel() const { return level_; }
bool ILoabotBattery::isCharging() const { return charging_; }
