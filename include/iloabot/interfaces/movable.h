#pragma once

struct Position {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
};

class Movable {
public:
    virtual ~Movable() = default;

    virtual void moveTo(double x, double y, double z) = 0;
    virtual void stop() = 0;
    virtual Position currentPosition() const = 0;
};
