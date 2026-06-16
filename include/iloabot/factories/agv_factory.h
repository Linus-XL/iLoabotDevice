#pragma once

#include "iloabot/product_factory.h"
#include "iloabot/devices/agv.h"

namespace iloabot::factories {

class AGVFactory final : public TypedProductFactory<ILoabotAGV> {
public:
    using ProductType = ILoabotAGV;

    static constexpr const char* Model002 = "A-200";

    std::string typeName() const override;
    std::vector<std::string> supportedModels() const override;
    std::string defaultModel() const override;
    std::unique_ptr<DeviceProduct> create(const std::string& model) const override;
};

} // namespace iloabot::factories
