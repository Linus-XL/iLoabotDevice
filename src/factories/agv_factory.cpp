#include "iloabot/factories/agv_factory.h"

#include "iloabot/devices/agv.h"
#include "iloabot/devices/agv002.h"

namespace iloabot::factories {

std::string AGVFactory::typeName() const { return "AGV"; }
std::vector<std::string> AGVFactory::supportedModels() const { return {Model002}; }
std::string AGVFactory::defaultModel() const { return Model002; }
std::unique_ptr<DeviceProduct> AGVFactory::create(const std::string& model) const {
	const auto selectedModel = model.empty() ? defaultModel() : model;
	if (selectedModel == Model002) {
		return std::make_unique<ILoabotAGV002>(selectedModel);
	}
	return nullptr;
}

} // namespace iloabot::factories
