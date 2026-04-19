#pragma once

#include <memory>
#include <string>

class Device {
public:
    virtual ~Device() = default;

    virtual std::string name() const = 0;
    virtual std::string type() const = 0;
};
