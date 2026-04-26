#pragma once

#include "iloabot/devices/suction_cup.h"

class ILoabotSuctionCup002 final : public ILoabotSuctionCup {
public:
    explicit ILoabotSuctionCup002(std::string model = "SC-200");

    std::string name() const override;
    bool initialize() override;
    void turnOn() override;
};