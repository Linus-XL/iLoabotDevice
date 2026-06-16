#include "iloabot/factories/charger_factory.h"

#include "iloabot/devices/charger.h"
#include "iloabot/devices/charger002.h"

namespace iloabot::factories {

std::string ChargerFactory::typeName() const { return "Charger"; }
std::vector<std::string> ChargerFactory::supportedModels() const { return {Model002}; }
std::string ChargerFactory::defaultModel() const { return Model002; }
std::unique_ptr<DeviceProduct> ChargerFactory::create(const std::string& model) const {
	const auto selectedModel = model.empty() ? defaultModel() : model;
	if (selectedModel == Model002) {
		return std::make_unique<ILoabotCharger002>(selectedModel);
	}
	return nullptr;
}

} // namespace iloabot::factories
