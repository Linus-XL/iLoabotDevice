#pragma once

#include "iloabot/device_product.h"

class ILoabotBattery : public DeviceProduct {
public:
    explicit ILoabotBattery(std::string model = "B-200");

    virtual ~ILoabotBattery() = default;

    virtual std::string name() const = 0;
    std::string type() const override;
    std::string model() const override;

    // 状态
    virtual DeviceStatus status() const;

    // 电源
    virtual int batteryLevel() const;
    virtual bool isCharging() const;

protected:
    std::string model_;
    DeviceStatus status_ = DeviceStatus::Idle;
    int level_ = 100;
    bool charging_ = false;
};
