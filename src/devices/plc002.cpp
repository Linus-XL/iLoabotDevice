#include "iloabot/devices/plc002.h"

ILoabotPLC002::ILoabotPLC002(std::string model)
    : ILoabotPLC(std::move(model)) {}

std::string ILoabotPLC002::name() const {
    return "iLoabot PLC 002 [" + model_ + "]";
}

void ILoabotPLC002::setParam(const std::string& key, const std::string& value) {
    params_["002_" + key] = value;
}
