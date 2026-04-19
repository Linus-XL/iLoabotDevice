#pragma once

#include <unordered_map>

#include "iloabot/device.h"
#include "iloabot/interfaces/controllable.h"
#include "iloabot/interfaces/status_provider.h"
#include "iloabot/interfaces/configurable.h"

class ILoabotPLC final : public Device, public Controllable, public StatusProvider, public Configurable {
public:
    std::string name() const override;
    std::string type() const override;

    bool initialize() override;
    void shutdown() override;
    void reset() override;

    DeviceStatus status() const override;

    void setParam(const std::string& key, const std::string& value) override;
    std::string getParam(const std::string& key) const override;

private:
    DeviceStatus status_ = DeviceStatus::Offline;
    std::unordered_map<std::string, std::string> params_;
};
