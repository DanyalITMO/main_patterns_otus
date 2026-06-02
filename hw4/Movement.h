#pragma once

#include "IMovable.h"

#include <memory>
#include <ICommand.h>

class Movement : public ICommand
{
public:

    Movement(const IMovablePtr& obj)
        : _obj(obj) {}

    void execute() override {
        auto position = _obj->getLocation();
        auto velocity = _obj->getVelocity();
        _obj->setLocation(position + velocity);
    }

private:

    IMovablePtr _obj;
};
