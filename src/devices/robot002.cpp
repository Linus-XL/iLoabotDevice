#include "iloabot/devices/robot002.h"

ILoabotRobot002::ILoabotRobot002(std::string model)
    : ILoabotRobot(std::move(model)) {}

std::string ILoabotRobot002::name() const {
    return "iLoabot Robot 002 [" + model_ + "]";
}

void ILoabotRobot002::moveTo(double x, double y, double z) {
    status_ = DeviceStatus::Running;
    position_ = {x + 1.0, y + 1.0, z};
    status_ = DeviceStatus::Idle;
}
