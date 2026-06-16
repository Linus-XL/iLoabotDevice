#pragma once

#include "iloabot/device_product.h"

class ILoabotCharger : public DeviceProduct {
public:
    explicit ILoabotCharger(std::string model = "C-200");

    virtual ~ILoabotCharger() = default;

    virtual std::string name() const = 0;
    std::string type() const override;
    std::string model() const override;

    // 控制
    virtual bool initialize();
    virtual void shutdown();
    virtual void reset();

    // 状态
    virtual DeviceStatus status() const;

    // 开关
    virtual void turnOn();
    virtual void turnOff();
    virtual bool isOn() const;

protected:
    std::string model_;
    DeviceStatus status_ = DeviceStatus::Offline;
    bool on_ = false;
};
