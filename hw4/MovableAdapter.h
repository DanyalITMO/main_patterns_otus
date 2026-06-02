#pragma once

#include "IMovable.h"
#include "UObject.h"
#include <memory>

#include <cmath>

class MovableAdapter : public IMovable
{
public:

    MovableAdapter(UObject& s)
        : _obj{s} {};

    Vector getLocation() override { return std::any_cast<Vector>(_obj.get_property("location")); }

    void setLocation(const Vector& location) override { _obj.set_property("location", location); }

    Vector getVelocity() override {
        auto direction = std::any_cast<int>(_obj.get_property("direction"));
        auto direction_number = std::any_cast<int>(_obj.get_property("directionNumber"));
        auto velocity = std::any_cast<int>(_obj.get_property("velocity"));
        auto angle = static_cast<double>(direction) / 360 * direction_number;
        return Vector{static_cast<int>(velocity * std::cos(angle)), static_cast<int>(velocity * std::sin(angle))};
    }

private:

    UObject& _obj;
};
