#pragma once
#include "ICommand.h"
#include "IFuelable.h"

//4. Реализована команда BurnFuelCommand
class BurnFuelCommand : public ICommand
{
public:
    BurnFuelCommand(const IFuelablePtr& ptr, int fuelConsumption)
        : _obj{ptr}
        , _fuelConsumption{fuelConsumption} {}

    void execute() override {
        _obj->setFuel(_obj->getFuel() - _fuelConsumption);
    }

private:
    IFuelablePtr _obj;
    int _fuelConsumption;
};