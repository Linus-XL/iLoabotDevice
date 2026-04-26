#pragma once

#include "iloabot/product_factory.h"
#include "iloabot/devices/suction_cup.h"

namespace iloabot::factories {

class SuctionCupFactory final : public TypedProductFactory<ILoabotSuctionCup> {
public:
    using ProductType = ILoabotSuctionCup;

    static constexpr const char* Model002 = "SC-200";

    std::string typeName() const override;
    std::vector<std::string> supportedModels() const override;
    std::string defaultModel() const override;
    std::unique_ptr<DeviceProduct> create(const std::string& model) const override;
};

} // namespace iloabot::factories
