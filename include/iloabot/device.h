#pragma once

#include "iloabot/device_product.h"

#include <memory>
#include <string>
#include <vector>

struct DeviceBuildSpec {
    std::string robotModel = "R-200";
    std::string agvModel = "A-200";
    std::string suctionCupModel = "SC-200";
    std::string batteryModel = "B-200";
    std::string chargerModel = "C-200";
    std::string lightModel = "L-200";
    std::string plcModel = "P-200";
};

class AssembledDevice {
public:
    explicit AssembledDevice(std::string name) : name_(std::move(name)) {}

    const std::string& name() const { return name_; }

    void addProduct(std::unique_ptr<DeviceProduct> product) {
        products_.push_back(std::move(product));
    }

    const std::vector<std::unique_ptr<DeviceProduct>>& products() const {
        return products_;
    }

private:
    std::string name_;
    std::vector<std::unique_ptr<DeviceProduct>> products_; 
};

class Device {
public:
    virtual ~Device() = default;

    virtual std::string name() const = 0;
    virtual std::unique_ptr<AssembledDevice> assemble(const DeviceBuildSpec& spec) const = 0;
};
