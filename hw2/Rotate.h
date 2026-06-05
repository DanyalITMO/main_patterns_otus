#pragma once

#include "IRotatable.h"

class Rotate
{
public:

    Rotate(IRotatablePtr&& obj)
        : _obj(std::move(obj)) {}

    void execute() {
        auto angle = _obj->getAngle();
        auto angularVelocity = _obj->getAngularVelocity();
        _obj->setAngle(angle + angularVelocity);
    }

private:

    IRotatablePtr _obj;
};