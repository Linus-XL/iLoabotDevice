#pragma once

#include "iloabot/device_product.h"

class ILoabotRobot : public DeviceProduct {
public:
    explicit ILoabotRobot(std::string model = "R-200");

    virtual ~ILoabotRobot() = default;

    virtual std::string name() const = 0;
    std::string type() const override;
    std::string model() const override;

    // 控制
    virtual bool initialize();
    virtual void shutdown();
    virtual void reset();

    // 状态
    virtual DeviceStatus status() const;

    // 运动
    virtual void moveTo(double x, double y, double z);
    virtual void stop();
    virtual Position currentPosition() const;

protected:
    std::string model_;
    DeviceStatus status_ = DeviceStatus::Offline;
    Position position_;
};
