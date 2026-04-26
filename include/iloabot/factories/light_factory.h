#pragma once

#include "iloabot/product_factory.h"
#include "iloabot/devices/light.h"

namespace iloabot::factories {

class LightFactory final : public TypedProductFactory<ILoabotLight> {
public:
    using ProductType = ILoabotLight;

    static constexpr const char* Model002 = "L-200";

    std::string typeName() const override;
    std::vector<std::string> supportedModels() const override;
    std::string defaultModel() const override;
    std::unique_ptr<DeviceProduct> create(const std::string& model) const override;
};

} // namespace iloabot::factories
