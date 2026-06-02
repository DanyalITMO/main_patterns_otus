#pragma once

#include "BurnFuelCommand.h"
#include "CheckFuelCommand.h"
#include "IFuelable.h"
#include "IMemento.h"
#include "IMovable.h"
#include "Macrocommand.h"
#include "Movement.h"

#include <memory>
#include <vector>

class IMovableWithFuel
    : public IMovable
    , public IFuelable
    , public IMemento
{};

using IMovableWithFuelPtr = std::shared_ptr<IMovableWithFuel>;

//6. Реализована макрокоманда движения по прямой с расходом топлива
class MoveMacrocommand : public Macrocommand
{
public:

    MoveMacrocommand(const IMovableWithFuelPtr& ptr, int fuelConsumption)
        : Macrocommand(std::vector<ICommandPtr>{std::make_shared<CheckFuelCommand>(ptr, fuelConsumption),
                                                std::make_shared<Movement>(ptr),
                                                std::make_shared<BurnFuelCommand>(ptr, fuelConsumption)})
        , _ptr{ptr} {}

    void execute() override {
        auto state_before = _ptr->create_memento();
        try {
            Macrocommand::execute();
        } catch (...) {
            _ptr->restore_from_memento(state_before);
            throw;
        }
    }

private:
    IMovableWithFuelPtr _ptr;
};
