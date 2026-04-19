#pragma once

class PowerManageable {
public:
    virtual ~PowerManageable() = default;

    virtual int batteryLevel() const = 0;
    virtual bool isCharging() const = 0;
};
