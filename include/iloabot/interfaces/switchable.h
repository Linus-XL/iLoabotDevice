#pragma once

class Switchable {
public:
    virtual ~Switchable() = default;

    virtual void turnOn() = 0;
    virtual void turnOff() = 0;
    virtual bool isOn() const = 0;
};
