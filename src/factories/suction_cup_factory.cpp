#include "iloabot/factories/suction_cup_factory.h"

#include "iloabot/devices/suction_cup.h"
#include "iloabot/devices/suction_cup002.h"

namespace iloabot::factories {

std::string SuctionCupFactory::typeName() const { return "SuctionCup"; }
std::vector<std::string> SuctionCupFactory::supportedModels() const { return {Model002}; }
std::string SuctionCupFactory::defaultModel() const { return Model002; }
std::unique_ptr<DeviceProduct> SuctionCupFactory::create(const std::string& model) const {
	const auto selectedModel = model.empty() ? defaultModel() : model;

	if (selectedModel == Model002) {
		return std::make_unique<ILoabotSuctionCup002>(selectedModel);
	}

	return nullptr;
}

} // namespace iloabot::factories
