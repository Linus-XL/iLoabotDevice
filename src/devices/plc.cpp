#include "iloabot/devices/plc.h"

std::string ILoabotPLC::name() const { return "iLoabot PLC"; }
std::string ILoabotPLC::type() const { return "PLC"; }

bool ILoabotPLC::initialize() {
    status_ = DeviceStatus::Idle;
    params_.clear();
    return true;
}

void ILoabotPLC::shutdown() {
    status_ = DeviceStatus::Offline;
}

void ILoabotPLC::reset() {
    params_.clear();
    status_ = DeviceStatus::Idle;
}

DeviceStatus ILoabotPLC::status() const { return status_; }

void ILoabotPLC::setParam(const std::string& key, const std::string& value) {
    params_[key] = value;
}

std::string ILoabotPLC::getParam(const std::string& key) const {
    auto it = params_.find(key);
    if (it != params_.end()) {
        return it->second;
    }
    return {};
}
