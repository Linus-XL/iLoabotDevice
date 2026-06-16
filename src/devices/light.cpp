#include "iloabot/devices/light.h"

ILoabotLight::ILoabotLight(std::string model) : model_(std::move(model)) {}

std::string ILoabotLight::type() const { return "Light"; }
std::string ILoabotLight::model() const { return model_; }

void ILoabotLight::turnOn() { on_ = true; }
void ILoabotLight::turnOff() { on_ = false; }
bool ILoabotLight::isOn() const { return on_; }
