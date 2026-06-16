#include "iloabot/devices/iloabot_device.h"

#include "iloabot/devices/agv.h"
#include "iloabot/devices/battery.h"
#include "iloabot/devices/charger.h"
#include "iloabot/devices/light.h"
#include "iloabot/devices/plc.h"
#include "iloabot/devices/robot.h"
#include "iloabot/devices/suction_cup.h"
#include "iloabot/factories/agv_factory.h"
#include "iloabot/factories/battery_factory.h"
#include "iloabot/factories/charger_factory.h"
#include "iloabot/factories/light_factory.h"
#include "iloabot/factories/plc_factory.h"
#include "iloabot/factories/robot_factory.h"
#include "iloabot/factories/suction_cup_factory.h"

ILoabotDevice::ILoabotDevice(const iloabot::factories::ProductFactoryRegistry& registry)
    : registry_(&registry) {}

std::string ILoabotDevice::name() const { return "iLoabot Device"; }

std::unique_ptr<AssembledDevice> ILoabotDevice::assemble(const DeviceBuildSpec& spec) const {
    auto device = std::make_unique<AssembledDevice>(name());

    device->addProduct(registry_->createByFactoryAs<iloabot::factories::RobotFactory, ILoabotRobot>(spec.robotModel));
    device->addProduct(registry_->createByFactoryAs<iloabot::factories::AGVFactory, ILoabotAGV>(spec.agvModel));
    device->addProduct(registry_->createByFactoryAs<iloabot::factories::SuctionCupFactory, ILoabotSuctionCup>(spec.suctionCupModel));
    device->addProduct(registry_->createByFactoryAs<iloabot::factories::BatteryFactory, ILoabotBattery>(spec.batteryModel));
    device->addProduct(registry_->createByFactoryAs<iloabot::factories::ChargerFactory, ILoabotCharger>(spec.chargerModel));
    device->addProduct(registry_->createByFactoryAs<iloabot::factories::LightFactory, ILoabotLight>(spec.lightModel));
    device->addProduct(registry_->createByFactoryAs<iloabot::factories::PLCFactory, ILoabotPLC>(spec.plcModel));

    return device;
}