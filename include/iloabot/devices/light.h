#pragma once

#include "iloabot/device_product.h"

class ILoabotLight : public DeviceProduct {
public:
    explicit ILoabotLight(std::string model = "L-200");

    virtual ~ILoabotLight() = default;

    virtual std::string name() const = 0;
    std::string type() const override;
    std::string model() const override;

    // 开关
    virtual void turnOn();
    virtual void turnOff();
    virtual bool isOn() const;

protected:
    std::string model_;
    bool on_ = false;
};
