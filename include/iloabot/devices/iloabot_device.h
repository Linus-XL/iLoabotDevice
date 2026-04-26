#pragma once

#include "iloabot/device.h"
#include "iloabot/product_factory.h"

class ILoabotDevice final : public Device {
public:
    explicit ILoabotDevice(
        const iloabot::factories::ProductFactoryRegistry& registry =
            iloabot::factories::ProductFactoryRegistry::instance());

    std::string name() const override;
    std::unique_ptr<AssembledDevice> assemble(const DeviceBuildSpec& spec) const override;

private:
    const iloabot::factories::ProductFactoryRegistry* registry_;
};