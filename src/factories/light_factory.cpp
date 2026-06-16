#include "iloabot/factories/light_factory.h"

#include "iloabot/devices/light.h"
#include "iloabot/devices/light002.h"

namespace iloabot::factories {

std::string LightFactory::typeName() const { return "Light"; }
std::vector<std::string> LightFactory::supportedModels() const { return {Model002}; }
std::string LightFactory::defaultModel() const { return Model002; }
std::unique_ptr<DeviceProduct> LightFactory::create(const std::string& model) const {
	const auto selectedModel = model.empty() ? defaultModel() : model;
	if (selectedModel == Model002) {
		return std::make_unique<ILoabotLight002>(selectedModel);
	}
	return nullptr;
}

} // namespace iloabot::factories
