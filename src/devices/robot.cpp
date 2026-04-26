#include "iloabot/devices/robot.h"

ILoabotRobot::ILoabotRobot(std::string model) : model_(std::move(model)) {}

std::string ILoabotRobot::type() const { return "Robot"; }
std::string ILoabotRobot::model() const { return model_; }

bool ILoabotRobot::initialize() {
    status_ = DeviceStatus::Idle;
    position_ = {0.0, 0.0, 0.0};
    return true;
}

void ILoabotRobot::shutdown() {
    status_ = DeviceStatus::Offline;
}

void ILoabotRobot::reset() {
    position_ = {0.0, 0.0, 0.0};
    status_ = DeviceStatus::Idle;
}

DeviceStatus ILoabotRobot::status() const { return status_; }

void ILoabotRobot::moveTo(double x, double y, double z) {
    status_ = DeviceStatus::Running;
    position_ = {x, y, z};
    status_ = DeviceStatus::Idle;
}

void ILoabotRobot::stop() {
    status_ = DeviceStatus::Idle;
}

Position ILoabotRobot::currentPosition() const { return position_; }
