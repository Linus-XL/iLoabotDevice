#pragma once

#include "iloabot/devices/charger.h"

class ILoabotCharger002 final : public ILoabotCharger {
public:
    explicit ILoabotCharger002(std::string model = "C-200");

    std::string name() const override;
    void turnOn() override;
};
