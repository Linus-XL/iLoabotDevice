#pragma once

class Controllable {
public:
    virtual ~Controllable() = default;

    virtual bool initialize() = 0;
    virtual void shutdown() = 0;
    virtual void reset() = 0;
};
