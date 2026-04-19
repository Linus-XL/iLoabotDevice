#pragma once

#include "iloabot/device.h"
#include "iloabot/interfaces/controllable.h"
#include "iloabot/interfaces/status_provider.h"
#include "iloabot/interfaces/movable.h"

class ILoabotAGV final : public Device, public Controllable, public StatusProvider, public Movable {
public:
    std::string name() const override;
    std::string type() const override;

    bool initialize() override;
    void shutdown() override;
    void reset() override;

    DeviceStatus status() const override;

    void moveTo(double x, double y, double z) override;
    void stop() override;
    Position currentPosition() const override;

private:
    DeviceStatus status_ = DeviceStatus::Offline;
    Position position_;
};
