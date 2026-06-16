#include "iloabot/factories/battery_factory.h"

#include "iloabot/devices/battery.h"
#include "iloabot/devices/battery002.h"

namespace iloabot::factories {

std::string BatteryFactory::typeName() const { return "Battery"; }
std::vector<std::string> BatteryFactory::supportedModels() const { return {Model002}; }
std::string BatteryFactory::defaultModel() const { return Model002; }
std::unique_ptr<DeviceProduct> BatteryFactory::create(const std::string& model) const {
	const auto selectedModel = model.empty() ? defaultModel() : model;
	if (selectedModel == Model002) {
		return std::make_unique<ILoabotBattery002>(selectedModel);
	}
	return nullptr;
}

} // namespace iloabot::factories
