#pragma once

#include "device.h"

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <iostream>

class DeviceFactory {
public:
    using Creator = std::function<std::unique_ptr<Device>()>;

    static DeviceFactory& instance();

    void registerDevice(const std::string& typeName, Creator creator);
    std::unique_ptr<Device> create(const std::string& typeName) const;

private:
    DeviceFactory() = default;
    std::unordered_map<std::string, Creator> creators_;
};

void registerILoabotDevices();
