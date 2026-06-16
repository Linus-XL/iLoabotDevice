#pragma once

#include <string>

// ── 设备通用状态 ───────────────────────────────────────────────────────────────
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

// ── 空间位置 ───────────────────────────────────────────────────────────────────
struct Position {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
};
