#pragma once

#include "iloabot/product_factory.h"
#include "iloabot/devices/charger.h"

namespace iloabot::factories {

class ChargerFactory final : public TypedProductFactory<ILoabotCharger> {
public:
    using ProductType = ILoabotCharger;

    static constexpr const char* Model002 = "C-200";

    std::string typeName() const override;
    std::vector<std::string> supportedModels() const override;
    std::string defaultModel() const override;
    std::unique_ptr<DeviceProduct> create(const std::string& model) const override;
};

} // namespace iloabot::factories
