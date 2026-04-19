#include "iloabot/device_factory.h"

#include "iloabot/devices/robot.h"
#include "iloabot/devices/agv.h"
#include "iloabot/devices/suction_cup.h"
#include "iloabot/devices/battery.h"
#include "iloabot/devices/charger.h"
#include "iloabot/devices/light.h"
#include "iloabot/devices/plc.h"

DeviceFactory& DeviceFactory::instance() {
    static DeviceFactory factory;
    return factory;
}

void DeviceFactory::registerDevice(const std::string& typeName, Creator creator) {
    creators_[typeName] = std::move(creator);
}

std::unique_ptr<Device> DeviceFactory::create(const std::string& typeName) const {
    auto it = creators_.find(typeName);
    if (it != creators_.end()) {
        return it->second();
    }
    std::cerr << "Unknown device type: " << typeName << "\n";
    return nullptr;
}

void registerILoabotDevices() {
    auto& factory = DeviceFactory::instance();
    factory.registerDevice("Robot",      [] { return std::make_unique<ILoabotRobot>(); });
    factory.registerDevice("AGV",        [] { return std::make_unique<ILoabotAGV>(); });
    factory.registerDevice("SuctionCup", [] { return std::make_unique<ILoabotSuctionCup>(); });
    factory.registerDevice("Battery",    [] { return std::make_unique<ILoabotBattery>(); });
    factory.registerDevice("Charger",    [] { return std::make_unique<ILoabotCharger>(); });
    factory.registerDevice("Light",      [] { return std::make_unique<ILoabotLight>(); });
    factory.registerDevice("PLC",        [] { return std::make_unique<ILoabotPLC>(); });
}
