#pragma once

#include "iloabot/device.h"
#include "iloabot/interfaces/switchable.h"

class ILoabotLight final : public Device, public Switchable {
public:
    std::string name() const override;
    std::string type() const override;

    void turnOn() override;
    void turnOff() override;
    bool isOn() const override;

private:
    bool on_ = false;
};
