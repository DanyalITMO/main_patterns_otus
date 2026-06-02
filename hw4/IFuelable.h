#pragma once
#include <memory>

class IFuelable
{
    public:
    virtual void setFuel(int) = 0;
    virtual int getFuel() = 0;
};

using IFuelablePtr = std::shared_ptr<IFuelable>;
