#pragma once

#include "iloabot/product_factory.h"
#include "iloabot/devices/battery.h"

namespace iloabot::factories {

class BatteryFactory final : public TypedProductFactory<ILoabotBattery> {
public:
    using ProductType = ILoabotBattery;

    static constexpr const char* Model002 = "B-200";

    std::string typeName() const override;
    std::vector<std::string> supportedModels() const override;
    std::string defaultModel() const override;
    std::unique_ptr<DeviceProduct> create(const std::string& model) const override;
};

} // namespace iloabot::factories
