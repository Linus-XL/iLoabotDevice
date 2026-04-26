#include "iloabot/device_factory.h"

#include "iloabot/factories/agv_factory.h"
#include "iloabot/factories/battery_factory.h"
#include "iloabot/factories/charger_factory.h"
#include "iloabot/factories/light_factory.h"
#include "iloabot/factories/plc_factory.h"
#include "iloabot/factories/robot_factory.h"
#include "iloabot/factories/suction_cup_factory.h"

namespace iloabot::factories {

namespace {

void eraseProductIfMatches(
    std::unordered_map<std::type_index, const ProductFactory*>& productFactories,
    std::type_index key,
    const ProductFactory* expected) {
    auto it = productFactories.find(key);
    if (it != productFactories.end() && it->second == expected) {
        productFactories.erase(it);
    }
}

void eraseFactoryTypeIfMatches(
    std::unordered_map<std::type_index, const ProductFactory*>& factoryTypeFactories,
    std::type_index key,
    const ProductFactory* expected) {
    auto it = factoryTypeFactories.find(key);
    if (it != factoryTypeFactories.end() && it->second == expected) {
        factoryTypeFactories.erase(it);
    }
}

} // namespace

ProductFactoryRegistry& ProductFactoryRegistry::instance() {
    static ProductFactoryRegistry registry;
    return registry;
}

void ProductFactoryRegistry::registerFactory(std::unique_ptr<ProductFactory> factory) {
    const auto key = factory->typeName();
    const auto productKey = factory->productTypeKey();
    const auto factoryTypeKey = std::type_index(typeid(*factory));

    if (auto it = factories_.find(key); it != factories_.end()) {
        const auto* oldFactory = it->second.get();
        eraseProductIfMatches(productFactories_, oldFactory->productTypeKey(), oldFactory);
        eraseFactoryTypeIfMatches(factoryTypeFactories_, std::type_index(typeid(*oldFactory)), oldFactory);
    }

    factories_[key] = std::move(factory);
    productFactories_[productKey] = factories_[key].get();
    factoryTypeFactories_[factoryTypeKey] = factories_[key].get();
}

bool ProductFactoryRegistry::unregisterFactory(const std::string& typeName) {
    auto it = factories_.find(typeName);
    if (it == factories_.end()) {
        return false;
    }

    const auto* factory = it->second.get();
    eraseProductIfMatches(productFactories_, factory->productTypeKey(), factory);
    eraseFactoryTypeIfMatches(factoryTypeFactories_, std::type_index(typeid(*factory)), factory);
    factories_.erase(it);
    return true;
}

void ProductFactoryRegistry::clear() {
    factories_.clear();
    productFactories_.clear();
    factoryTypeFactories_.clear();
}

const ProductFactory* ProductFactoryRegistry::findFactory(const std::string& typeName) const {
    auto it = factories_.find(typeName);
    if (it != factories_.end()) {
        return it->second.get();
    }
    return nullptr;
}

std::unique_ptr<DeviceProduct> ProductFactoryRegistry::create(
    const std::string& typeName,
    const std::string& model) const {
    const auto* factory = findFactory(typeName);
    if (!factory) {
        std::cerr << "Unknown device type: " << typeName << "\n";
        return nullptr;
    }

    return factory->create(model);
}

void registerILoabotProductFactories() {
    auto& registry = ProductFactoryRegistry::instance();
    registry.registerFactory(std::make_unique<RobotFactory>());
    registry.registerFactory(std::make_unique<AGVFactory>());
    registry.registerFactory(std::make_unique<SuctionCupFactory>());
    registry.registerFactory(std::make_unique<BatteryFactory>());
    registry.registerFactory(std::make_unique<ChargerFactory>());
    registry.registerFactory(std::make_unique<LightFactory>());
    registry.registerFactory(std::make_unique<PLCFactory>());
}

} // namespace iloabot::factories
