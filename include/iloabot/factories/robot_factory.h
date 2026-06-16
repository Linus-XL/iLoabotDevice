#pragma once

#include "iloabot/product_factory.h"
#include "iloabot/devices/robot.h"

namespace iloabot::factories {

class RobotFactory final : public TypedProductFactory<ILoabotRobot> {
public:
    using ProductType = ILoabotRobot;

    static constexpr const char* Model002 = "R-200";

    std::string typeName() const override;
    std::vector<std::string> supportedModels() const override;
    std::string defaultModel() const override;
    std::unique_ptr<DeviceProduct> create(const std::string& model) const override;
};

} // namespace iloabot::factories
