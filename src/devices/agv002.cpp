#include "iloabot/devices/agv002.h"

ILoabotAGV002::ILoabotAGV002(std::string model)
    : ILoabotAGV(std::move(model)) {}

std::string ILoabotAGV002::name() const {
    return "iLoabot AGV 002 [" + model_ + "]";
}

void ILoabotAGV002::moveTo(double x, double y, double z) {
    status_ = DeviceStatus::Running;
    position_ = {x, y, z + 0.5};
    status_ = DeviceStatus::Idle;
}
