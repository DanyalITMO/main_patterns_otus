#pragma once

#include "IRotatable.h"
#include <ICommand.h>

class Rotate :  public ICommand
{
public:

    explicit Rotate(const IRotatablePtr& obj)
        : _obj(obj) {}

    void execute() override {
        auto angle = _obj->getAngle();
        auto angularVelocity = _obj->getAngularVelocity();
        _obj->setAngle(angle + angularVelocity);
    }

private:

    IRotatablePtr _obj;
};