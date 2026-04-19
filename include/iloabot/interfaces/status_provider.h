#pragma once

#include <string>

enum class DeviceStatus {
    Offline,
    Idle,
    Running,
    Error
};

inline std::string deviceStatusToString(DeviceStatus s) {
    switch (s) {
        case DeviceStatus::Offline: return "Offline";
        case DeviceStatus::Idle:    return "Idle";
        case DeviceStatus::Running: return "Running";
        case DeviceStatus::Error:   return "Error";
    }
    return "Unknown";
}

class StatusProvider {
public:
    virtual ~StatusProvider() = default;

    virtual DeviceStatus status() const = 0;
};
