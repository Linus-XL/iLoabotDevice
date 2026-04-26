#pragma once

#include "iloabot/product_factory.h"
#include "iloabot/devices/plc.h"

namespace iloabot::factories {

class PLCFactory final : public TypedProductFactory<ILoabotPLC> {
public:
    using ProductType = ILoabotPLC;

    static constexpr const char* Model002 = "P-200";

    std::string typeName() const override;
    std::vector<std::string> supportedModels() const override;
    std::string defaultModel() const override;
    std::unique_ptr<DeviceProduct> create(const std::string& model) const override;
};

} // namespace iloabot::factories
