#pragma once

#include "iloabot/devices/battery.h"

class ILoabotBattery002 final : public ILoabotBattery {
public:
    explicit ILoabotBattery002(std::string model = "B-200");

    std::string name() const override;
    int batteryLevel() const override;
};
