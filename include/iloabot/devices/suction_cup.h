#pragma once

#include "iloabot/device_product.h"

class ILoabotSuctionCup : public DeviceProduct {
public:
    explicit ILoabotSuctionCup(std::string model = "SC-200");

    virtual ~ILoabotSuctionCup() = default;

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
