#include "iloabot/devices/light002.h"

ILoabotLight002::ILoabotLight002(std::string model)
    : ILoabotLight(std::move(model)) {}

std::string ILoabotLight002::name() const {
    return "iLoabot Light 002 [" + model_ + "]";
}

void ILoabotLight002::turnOn() {
    on_ = true;
}
