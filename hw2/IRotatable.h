#pragma once

#include "angle.hpp"
#include <memory>

class IRotatable
{
public:

    virtual ~IRotatable() = default;
    virtual Angle getAngle() = 0;
    virtual void setAngle(const Angle& angle) = 0;
    virtual Angle getAngularVelocity() = 0;
};

using IRotatablePtr = std::shared_ptr<IRotatable>;
