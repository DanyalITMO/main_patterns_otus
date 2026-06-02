#pragma once

#include "IRotatable.h"
#include "UObject.h"

class RotatableAdapter : public IRotatable
{
public:

    explicit RotatableAdapter(UObject& obj)
        : _obj(obj) {}

    Angle getAngle() override { return std::any_cast<Angle>(_obj.get_property("angle")); }

    void setAngle(const Angle& angle) override { _obj.set_property("angle", angle); }

    Angle getAngularVelocity() override { return std::any_cast<Angle>(_obj.get_property("angularVelocity")); }

private:

    UObject& _obj;
};

using RotatableSpaceshipPtr = std::unique_ptr<RotatableAdapter>;
