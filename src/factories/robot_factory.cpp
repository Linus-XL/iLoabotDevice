#include "iloabot/factories/robot_factory.h"

#include "iloabot/devices/robot.h"
#include "iloabot/devices/robot002.h"

namespace iloabot::factories {

std::string RobotFactory::typeName() const { return "Robot"; }
std::vector<std::string> RobotFactory::supportedModels() const { return {Model002}; }
std::string RobotFactory::defaultModel() const { return Model002; }
std::unique_ptr<DeviceProduct> RobotFactory::create(const std::string& model) const {
	const auto selectedModel = model.empty() ? defaultModel() : model;
	if (selectedModel == Model002) {
		return std::make_unique<ILoabotRobot002>(selectedModel);
	}
	return nullptr;
}

} // namespace iloabot::factories
