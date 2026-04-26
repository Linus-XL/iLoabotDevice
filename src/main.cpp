#include "iloabot/product_factory.h"
#include "iloabot/devices/iloabot_device.h"
#include "iloabot/devices/suction_cup.h"
#include "iloabot/factories/suction_cup_factory.h"

#include <iostream>
#include <string>

int main() {
    // 初始化产品工厂
    iloabot::factories::registerILoabotProductFactories();

    auto& registry = iloabot::factories::ProductFactoryRegistry::instance();
    ILoabotDevice device(registry);

    DeviceBuildSpec spec;
    spec.robotModel = "R-200";
    spec.agvModel = "A-200";
    spec.suctionCupModel = "SC-200";
    spec.batteryModel = "B-200";
    spec.chargerModel = "C-200";
    spec.lightModel = "L-200";
    spec.plcModel = "P-200";

    auto assembled = device.assemble(spec);

    std::cout << assembled->name() << '\n';
    for (const auto& product : assembled->products()) {
        if (product) {
            std::cout << " - " << product->type() << " / " << product->model()
                      << " / " << product->name() << '\n';
        }
    }

    auto suctionCup = registry.createByFactoryAs<iloabot::factories::SuctionCupFactory, ILoabotSuctionCup>(
        iloabot::factories::SuctionCupFactory::Model002);
    if (suctionCup) {
        std::cout << "Factory direct product: " << suctionCup->name() << '\n';
    }

    auto selectedSuctionCup = registry.createByFactoryAs<iloabot::factories::SuctionCupFactory, ILoabotSuctionCup>(
        iloabot::factories::SuctionCupFactory::Model002);
    if (selectedSuctionCup) {
        std::cout << "Selected concrete implementation: " << selectedSuctionCup->name() << '\n';
    }

    return 0;
}
