#pragma once
#include "ICommand.h"
#include "IFuelable.h"
#include "CommandException.h"

//2. Реализована команда CheckFuelCommand
class CheckFuelCommand : public ICommand
{
public:
    CheckFuelCommand(const IFuelablePtr& ptr, int requiredFuel)
        : _obj{ptr}
        , _requiredFuel{requiredFuel} {}

    void execute() override {
        if (_obj->getFuel() < _requiredFuel) {
            throw CommandException("Fuel out of range");
        }
    }

private:
    IFuelablePtr _obj;
    int _requiredFuel;
};