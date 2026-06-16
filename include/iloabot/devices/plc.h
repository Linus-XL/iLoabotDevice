#pragma once

#include <string>
#include <unordered_map>

#include "iloabot/device_product.h"

class ILoabotPLC : public DeviceProduct {
public:
    explicit ILoabotPLC(std::string model = "P-200");

    virtual ~ILoabotPLC() = default;

    virtual std::string name() const = 0;
    std::string type() const override;
    std::string model() const override;

    // 控制
    virtual bool initialize();
    virtual void shutdown();
    virtual void reset();

    // 状态
    virtual DeviceStatus status() const;

    // 参数配置
    virtual void setParam(const std::string& key, const std::string& value);
    virtual std::string getParam(const std::string& key) const;

protected:
    std::string model_;
    DeviceStatus status_ = DeviceStatus::Offline;
    std::unordered_map<std::string, std::string> params_;
};
