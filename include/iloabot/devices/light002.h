#pragma once

#include "iloabot/devices/light.h"

class ILoabotLight002 final : public ILoabotLight {
public:
    explicit ILoabotLight002(std::string model = "L-200");

    std::string name() const override;
    void turnOn() override;
};
