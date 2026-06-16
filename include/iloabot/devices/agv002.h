#pragma once

#include "iloabot/devices/agv.h"

class ILoabotAGV002 final : public ILoabotAGV {
public:
    explicit ILoabotAGV002(std::string model = "A-200");

    std::string name() const override;
    void moveTo(double x, double y, double z) override;
};
