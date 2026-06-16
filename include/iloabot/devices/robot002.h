#pragma once

#include "iloabot/devices/robot.h"

class ILoabotRobot002 final : public ILoabotRobot {
public:
    explicit ILoabotRobot002(std::string model = "R-200");

    std::string name() const override;
    void moveTo(double x, double y, double z) override;
};
