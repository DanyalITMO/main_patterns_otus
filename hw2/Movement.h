#pragma once

#include "IMovable.h"

#include <memory>

class Movement
{
public:

    Movement(const IMovablePtr& obj)
        : _obj(obj) {}

    void execute() {
        auto position = _obj->getLocation();
        auto velocity = _obj->getVelocity();
        _obj->setLocation(position + velocity);
    }

private:

    IMovablePtr _obj;
};
