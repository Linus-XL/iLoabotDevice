#pragma once

#include "iloabot/device_types.h"

#include <memory>
#include <string>

class DeviceProduct {
public:
    virtual ~DeviceProduct() = default;

    virtual std::string name() const = 0;
    virtual std::string type() const = 0;
    virtual std::string model() const = 0;
};