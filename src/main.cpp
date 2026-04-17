#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Robot {
public:
    virtual ~Robot() = default;
    virtual std::string name() const = 0;
};

class AGV {
public:
    virtual ~AGV() = default;
    virtual std::string name() const = 0;
};

class SuctionCup {
public:
    virtual ~SuctionCup() = default;
    virtual std::string name() const = 0;
};

class Battery {
public:
    virtual ~Battery() = default;
    virtual std::string name() const = 0;
};

class Charger {
public:
    virtual ~Charger() = default;
    virtual std::string name() const = 0;
};

class Light {
public:
    virtual ~Light() = default;
    virtual std::string name() const = 0;
};

class PLC {
public:
    virtual ~PLC() = default;
    virtual std::string name() const = 0;
};

class RobotFactory {
public:
    virtual ~RobotFactory() = default;
    virtual std::unique_ptr<Robot> create() const = 0;
};

class AGVFactory {
public:
    virtual ~AGVFactory() = default;
    virtual std::unique_ptr<AGV> create() const = 0;
};

class SuctionCupFactory {
public:
    virtual ~SuctionCupFactory() = default;
    virtual std::unique_ptr<SuctionCup> create() const = 0;
};

class BatteryFactory {
public:
    virtual ~BatteryFactory() = default;
    virtual std::unique_ptr<Battery> create() const = 0;
};

class ChargerFactory {
public:
    virtual ~ChargerFactory() = default;
    virtual std::unique_ptr<Charger> create() const = 0;
};

class LightFactory {
public:
    virtual ~LightFactory() = default;
    virtual std::unique_ptr<Light> create() const = 0;
};

class PLCFactory {
public:
    virtual ~PLCFactory() = default;
    virtual std::unique_ptr<PLC> create() const = 0;
};

class ILoabotFactory {
public:
    virtual ~ILoabotFactory() = default;
    virtual const RobotFactory& robotFactory() const = 0;
    virtual const AGVFactory& agvFactory() const = 0;
    virtual const SuctionCupFactory& suctionCupFactory() const = 0;
    virtual const BatteryFactory& batteryFactory() const = 0;
    virtual const ChargerFactory& chargerFactory() const = 0;
    virtual const LightFactory& lightFactory() const = 0;
    virtual const PLCFactory& plcFactory() const = 0;
};

class ILoabotRobot final : public Robot {
public:
    std::string name() const override { return "iLoabot Robot"; }
};

class ILoabotAGV final : public AGV {
public:
    std::string name() const override { return "iLoabot AGV"; }
};

class ILoabotSuctionCup final : public SuctionCup {
public:
    std::string name() const override { return "iLoabot SuctionCup"; }
};

class ILoabotBattery final : public Battery {
public:
    std::string name() const override { return "iLoabot Battery"; }
};

class ILoabotCharger final : public Charger {
public:
    std::string name() const override { return "iLoabot Charger"; }
};

class ILoabotLight final : public Light {
public:
    std::string name() const override { return "iLoabot Light"; }
};

class ILoabotPLC final : public PLC {
public:
    std::string name() const override { return "iLoabot PLC"; }
};

class ILoabotRobotFactory final : public RobotFactory {
public:
    std::unique_ptr<Robot> create() const override {
        return std::make_unique<ILoabotRobot>();
    }
};

class ILoabotAGVFactory final : public AGVFactory {
public:
    std::unique_ptr<AGV> create() const override {
        return std::make_unique<ILoabotAGV>();
    }
};

class ILoabotSuctionCupFactory final : public SuctionCupFactory {
public:
    std::unique_ptr<SuctionCup> create() const override {
        return std::make_unique<ILoabotSuctionCup>();
    }
};

class ILoabotBatteryFactory final : public BatteryFactory {
public:
    std::unique_ptr<Battery> create() const override {
        return std::make_unique<ILoabotBattery>();
    }
};

class ILoabotChargerFactory final : public ChargerFactory {
public:
    std::unique_ptr<Charger> create() const override {
        return std::make_unique<ILoabotCharger>();
    }
};

class ILoabotLightFactory final : public LightFactory {
public:
    std::unique_ptr<Light> create() const override {
        return std::make_unique<ILoabotLight>();
    }
};

class ILoabotPLCFactory final : public PLCFactory {
public:
    std::unique_ptr<PLC> create() const override {
        return std::make_unique<ILoabotPLC>();
    }
};

class ILoabotDeviceFactory final : public ILoabotFactory {
public:
    const RobotFactory& robotFactory() const override { return robotFactory_; }
    const AGVFactory& agvFactory() const override { return agvFactory_; }
    const SuctionCupFactory& suctionCupFactory() const override { return suctionCupFactory_; }
    const BatteryFactory& batteryFactory() const override { return batteryFactory_; }
    const ChargerFactory& chargerFactory() const override { return chargerFactory_; }
    const LightFactory& lightFactory() const override { return lightFactory_; }
    const PLCFactory& plcFactory() const override { return plcFactory_; }

private:
    ILoabotRobotFactory robotFactory_;
    ILoabotAGVFactory agvFactory_;
    ILoabotSuctionCupFactory suctionCupFactory_;
    ILoabotBatteryFactory batteryFactory_;
    ILoabotChargerFactory chargerFactory_;
    ILoabotLightFactory lightFactory_;
    ILoabotPLCFactory plcFactory_;
};

int main() {
    ILoabotDeviceFactory iLoabot;

    std::vector<std::string> devices;
    devices.push_back(iLoabot.robotFactory().create()->name());
    devices.push_back(iLoabot.agvFactory().create()->name());
    devices.push_back(iLoabot.suctionCupFactory().create()->name());
    devices.push_back(iLoabot.batteryFactory().create()->name());
    devices.push_back(iLoabot.chargerFactory().create()->name());
    devices.push_back(iLoabot.lightFactory().create()->name());
    devices.push_back(iLoabot.plcFactory().create()->name());

    for (const auto& d : devices) {
        std::cout << d << '\n';
    }

    return 0;
}
