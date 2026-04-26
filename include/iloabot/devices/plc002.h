#pragma once

#include "iloabot/devices/plc.h"

class ILoabotPLC002 final : public ILoabotPLC {
public:
    explicit ILoabotPLC002(std::string model = "P-200");

    std::string name() const override;
    void setParam(const std::string& key, const std::string& value) override;
};
