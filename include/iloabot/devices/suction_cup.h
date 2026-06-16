#pragma once

#include "iloabot/device_product.h"
#include<iostream>

class ILoabotSuctionCup : public DeviceProduct {
public:
    explicit ILoabotSuctionCup(std::string model = "SC-200");

    virtual ~ILoabotSuctionCup() = default;

    virtual std::string name() const = 0;
    std::string type() const override;
    std::string model() const override;

    // 控制
    virtual bool initialize();
    virtual void shutdown();
    virtual void reset();

    virtual void sendCommand(const std::string& command) {
        // 默认实现：打印命令
        std::cout << "Sending command to " << name() << ": " << command << std::endl;
    }
    virtual void receivemsg(std::string& msg) {
        // 默认实现：打印接收到的消息
        std::cout << "Received message from " << name() << ": " << msg << std::endl;
    }
    // 状态
    virtual DeviceStatus status() const;

    // 开关
    virtual void turnOn();
    virtual void turnOff();
    virtual bool isOn() const;

protected:
    std::string model_;
    DeviceStatus status_ = DeviceStatus::Offline;
    bool on_ = false;
};
