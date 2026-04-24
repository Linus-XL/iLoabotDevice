#include "iloabot/device_factory.h"

#include <iostream>
#include <string>
#include <vector>

int main() {
    // 初始化注册设备
    registerILoabotDevices();

    // 统一生成设备的列表
    std::vector<std::string> targetDevices = {
        "Robot", "AGV", "SuctionCup", "Battery", "Charger", "Light", "PLC"
    };

    auto& factory = DeviceFactory::instance();

    // 创建并调用
    for (const auto& devType : targetDevices) {
        if (auto device = factory.create(devType)) {
            std::cout << device->name() << '\n';
        }
    }
    auto device = factory.create("SuctionCup");
    
    return 0;
}
