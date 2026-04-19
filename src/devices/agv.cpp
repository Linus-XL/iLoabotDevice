#include "iloabot/devices/agv.h"

std::string ILoabotAGV::name() const { return "iLoabot AGV"; }
std::string ILoabotAGV::type() const { return "AGV"; }

bool ILoabotAGV::initialize() {
    status_ = DeviceStatus::Idle;
    position_ = {0.0, 0.0, 0.0};
    return true;
}

void ILoabotAGV::shutdown() {
    status_ = DeviceStatus::Offline;
}

void ILoabotAGV::reset() {
    position_ = {0.0, 0.0, 0.0};
    status_ = DeviceStatus::Idle;
}

DeviceStatus ILoabotAGV::status() const { return status_; }

void ILoabotAGV::moveTo(double x, double y, double z) {
    status_ = DeviceStatus::Running;
    position_ = {x, y, z};
    status_ = DeviceStatus::Idle;
}

void ILoabotAGV::stop() {
    status_ = DeviceStatus::Idle;
}

Position ILoabotAGV::currentPosition() const { return position_; }
