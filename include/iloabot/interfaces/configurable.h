#pragma once

#include <string>

class Configurable {
public:
    virtual ~Configurable() = default;

    virtual void setParam(const std::string& key, const std::string& value) = 0;
    virtual std::string getParam(const std::string& key) const = 0;
};
