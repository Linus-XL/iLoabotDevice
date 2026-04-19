#include "iloabot/devices/light.h"

std::string ILoabotLight::name() const { return "iLoabot Light"; }
std::string ILoabotLight::type() const { return "Light"; }

void ILoabotLight::turnOn() { on_ = true; }
void ILoabotLight::turnOff() { on_ = false; }
bool ILoabotLight::isOn() const { return on_; }
