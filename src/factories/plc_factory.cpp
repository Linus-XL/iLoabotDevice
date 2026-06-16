#include "iloabot/factories/plc_factory.h"

#include "iloabot/devices/plc.h"
#include "iloabot/devices/plc002.h"

namespace iloabot::factories {

std::string PLCFactory::typeName() const { return "PLC"; }
std::vector<std::string> PLCFactory::supportedModels() const { return {Model002}; }
std::string PLCFactory::defaultModel() const { return Model002; }
std::unique_ptr<DeviceProduct> PLCFactory::create(const std::string& model) const {
	const auto selectedModel = model.empty() ? defaultModel() : model;
	if (selectedModel == Model002) {
		return std::make_unique<ILoabotPLC002>(selectedModel);
	}
	return nullptr;
}

} // namespace iloabot::factories
