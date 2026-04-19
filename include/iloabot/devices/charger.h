#pragma once

#include "iloabot/device.h"
#include "iloabot/interfaces/controllable.h"
#include "iloabot/interfaces/status_provider.h"
#include "iloabot/interfaces/switchable.h"

class ILoabotCharger final : public Device, public Controllable, public StatusProvider, public Switchable {
public:
    std::string name() const override;
    std::string type() const override;

    bool initialize() override;
    void shutdown() override;
    void reset() override;

    DeviceStatus status() const override;

    void turnOn() override;
    void turnOff() override;
    bool isOn() const override;

private:
    DeviceStatus status_ = DeviceStatus::Offline;
    bool on_ = false;
};
