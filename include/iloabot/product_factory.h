#pragma once

#include "device_product.h"

#include <iostream>
#include <memory>
#include <string>
#include <typeindex>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace iloabot::factories {

class ProductFactory {
public:
    virtual ~ProductFactory() = default;

    virtual std::string typeName() const = 0;
    virtual std::type_index productTypeKey() const = 0;
    virtual std::vector<std::string> supportedModels() const = 0;
    virtual std::string defaultModel() const = 0;
    virtual std::unique_ptr<DeviceProduct> create(const std::string& model) const = 0;

    std::unique_ptr<DeviceProduct> create() const {
        return create(defaultModel());
    }

    template <typename T>
    std::unique_ptr<T> createAs(const std::string& model) const {
        static_assert(std::is_base_of_v<DeviceProduct, T>, "T must derive from DeviceProduct");

        auto base = create(model);
        if (!base) {
            return nullptr;
        }

        auto* derived = dynamic_cast<T*>(base.get());
        if (!derived) {
            std::cerr << "Type mismatch for product factory: " << typeName() << "\n";
            return nullptr;
        }

        base.release();
        return std::unique_ptr<T>(derived);
    }
};

template <typename ProductT>
class TypedProductFactory : public ProductFactory {
public:
    using ProductType = ProductT;

    std::type_index productTypeKey() const final {
        static_assert(std::is_base_of_v<DeviceProduct, ProductT>, "ProductT must derive from DeviceProduct");
        return std::type_index(typeid(ProductT));
    }
};

class ProductFactoryRegistry {
public:
    static ProductFactoryRegistry& instance();

    void registerFactory(std::unique_ptr<ProductFactory> factory);
    bool unregisterFactory(const std::string& typeName);
    void clear();

    const ProductFactory* findFactory(const std::string& typeName) const;
    std::unique_ptr<DeviceProduct> create(const std::string& typeName, const std::string& model) const;

    std::unique_ptr<DeviceProduct> create(const std::string& typeName) const {
        const auto* factory = findFactory(typeName);
        if (!factory) {
            std::cerr << "Unknown device type: " << typeName << "\n";
            return nullptr;
        }

        return factory->create();
    }

    template <typename FactoryT>
    const FactoryT* findFactoryByType() const {
        static_assert(std::is_base_of_v<ProductFactory, FactoryT>, "FactoryT must derive from ProductFactory");

        auto it = factoryTypeFactories_.find(std::type_index(typeid(FactoryT)));
        if (it != factoryTypeFactories_.end()) {
            return static_cast<const FactoryT*>(it->second);
        }
        return nullptr;
    }

    template <typename FactoryT>
    bool unregisterFactoryByType() {
        static_assert(std::is_base_of_v<ProductFactory, FactoryT>, "FactoryT must derive from ProductFactory");

        auto it = factoryTypeFactories_.find(std::type_index(typeid(FactoryT)));
        if (it == factoryTypeFactories_.end()) {
            return false;
        }

        return unregisterFactory(it->second->typeName());
    }

    template <typename FactoryT>
    std::unique_ptr<DeviceProduct> createByFactory(const std::string& model) const {
        static_assert(std::is_base_of_v<ProductFactory, FactoryT>, "FactoryT must derive from ProductFactory");

        const auto* factory = findFactoryByType<FactoryT>();
        if (!factory) {
            std::cerr << "Factory type not registered\n";
            return nullptr;
        }

        return factory->create(model);
    }

    template <typename FactoryT, typename ProductT>
    std::unique_ptr<ProductT> createByFactoryAs(const std::string& model) const {
        static_assert(std::is_base_of_v<ProductFactory, FactoryT>, "FactoryT must derive from ProductFactory");
        static_assert(std::is_base_of_v<DeviceProduct, ProductT>, "ProductT must derive from DeviceProduct");

        const auto* factory = findFactoryByType<FactoryT>();
        if (!factory) {
            std::cerr << "Factory type not registered\n";
            return nullptr;
        }

        return factory->template createAs<ProductT>(model);
    }

    template <typename ProductT>
    std::unique_ptr<ProductT> createByProduct(const std::string& model) const {
        static_assert(std::is_base_of_v<DeviceProduct, ProductT>, "ProductT must derive from DeviceProduct");

        auto it = productFactories_.find(std::type_index(typeid(ProductT)));
        if (it == productFactories_.end()) {
            std::cerr << "No factory for product type\n";
            return nullptr;
        }

        return it->second->template createAs<ProductT>(model);
    }

    template <typename T>
    std::unique_ptr<T> createAs(const std::string& typeName, const std::string& model) const {
        static_assert(std::is_base_of_v<DeviceProduct, T>, "T must derive from DeviceProduct");

        const auto* factory = findFactory(typeName);
        if (!factory) {
            std::cerr << "Unknown device type: " << typeName << "\n";
            return nullptr;
        }

        return factory->createAs<T>(model);
    }

private:
    ProductFactoryRegistry() = default;
    std::unordered_map<std::string, std::unique_ptr<ProductFactory>> factories_;
    std::unordered_map<std::type_index, const ProductFactory*> productFactories_;
    std::unordered_map<std::type_index, const ProductFactory*> factoryTypeFactories_;
};

void registerILoabotProductFactories();

} // namespace iloabot::factories
