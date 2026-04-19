#pragma once

#include "iloabot/device.h"
#include "iloabot/interfaces/status_provider.h"
#include "iloabot/interfaces/power_manageable.h"

class ILoabotBattery final : public Device, public StatusProvider, public PowerManageable {
public:
    std::string name() const override;
    std::string type() const override;

    DeviceStatus status() const override;

    int batteryLevel() const override;
    bool isCharging() const override;

private:
    DeviceStatus status_ = DeviceStatus::Idle;
    int level_ = 100;
    bool charging_ = false;
};
