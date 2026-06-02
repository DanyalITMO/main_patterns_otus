#include "Movement.h"
#include "Rotate.h"
#include "BurnFuelCommand.h"
#include "CheckFuelCommand.h"
#include "Macrocommand.h"
#include "IMovable.h"
#include <gtest/gtest.h>
#include <stdexcept>
#include <gmock/gmock.h>
#include <MoveMacrocommand.h>

using testing::Return;
using testing::Throw;
using testing::InSequence;

class FuelMock : public IFuelable
{
public:
    MOCK_METHOD(void, setFuel, (int), (override));
    MOCK_METHOD(int, getFuel, (), (override));
};

//3. Написаны тесты к CheckFuelCommand
TEST(CheckFuelCommand, DoesNotThrowWhenFuelIsEnough) {
    auto mock = std::make_shared<FuelMock>();
    EXPECT_CALL(*mock, getFuel()).WillOnce(Return(10));

    CheckFuelCommand command{mock, 7};

    EXPECT_NO_THROW(command.execute());
}

TEST(CheckFuelCommand, ThrowsCommandExceptionWhenFuelIsNotEnough) {
    auto mock = std::make_shared<FuelMock>();
    EXPECT_CALL(*mock, getFuel()).WillOnce(Return(3));

    CheckFuelCommand command{mock, 7};

    EXPECT_THROW(command.execute(), CommandException);
}

//5. Написаны тесты к BurnFuelCommand
TEST(BurnFuelCommand, BurnsFuelByConsumptionRate) {
    auto mock = std::make_shared<FuelMock>();
    EXPECT_CALL(*mock, getFuel()).WillOnce(Return(10));
    EXPECT_CALL(*mock, setFuel(6)).Times(1);

    BurnFuelCommand command{mock, 4};

    EXPECT_NO_THROW(command.execute());
}

//7. Написаны тесты к Macrocommand
class CommandMock : public ICommand
{
public:
    MOCK_METHOD(void, execute, (), (override));
};

TEST(Macrocommand, ExecutesCommandsSequentially) {
    auto first = std::make_shared<CommandMock>();
    auto second = std::make_shared<CommandMock>();

    {
        InSequence sequence;
        EXPECT_CALL(*first, execute()).Times(1);
        EXPECT_CALL(*second, execute()).Times(1);
    }

    Macrocommand command{{first, second}};

    EXPECT_NO_THROW(command.execute());
}

TEST(Macrocommand, StopsExecutionAndThrowsCommandExceptionWhenCommandFails) {
    auto first = std::make_shared<CommandMock>();
    auto second = std::make_shared<CommandMock>();
    auto third = std::make_shared<CommandMock>();

    {
        InSequence sequence;
        EXPECT_CALL(*first, execute()).Times(1);
        EXPECT_CALL(*second, execute()).WillOnce(Throw(std::runtime_error("failed")));
    }
    EXPECT_CALL(*third, execute()).Times(0);

    Macrocommand command{{first, second, third}};

    EXPECT_THROW(command.execute(), CommandException);
}

//6. Тесты макрокоманды движения по прямой с расходом топлива
class MovableFuelObject : public IMovableWithFuel
{
public:
    MovableFuelObject(Vector location, Vector velocity, int fuel)
        : _location{location}
        , _velocity{velocity}
        , _fuel{fuel} {}

    Vector getLocation() override { return _location; }
    void setLocation(const Vector& location) override { _location = location; }
    Vector getVelocity() override { return _velocity; }
    void setFuel(int fuel) override { _fuel = fuel; }
    int getFuel() override { return _fuel; }

    std::unique_ptr<Memento> create_memento() override {
        return std::make_unique<State>(_location, _velocity, _fuel);
    }

    void restore_from_memento(const std::unique_ptr<Memento>& memento) override {
        const auto& state = static_cast<const State&>(*memento);
        _location = state.location;
        _velocity = state.velocity;
        _fuel = state.fuel;
    }

private:
    struct State : Memento
    {
        State(Vector location, Vector velocity, int fuel)
            : location{location}
            , velocity{velocity}
            , fuel{fuel} {}

        Vector location;
        Vector velocity;
        int fuel;
    };

    Vector _location;
    Vector _velocity;
    int _fuel;
};

TEST(MoveMacrocommand, MovesObjectAndBurnsFuel) {
    auto object = std::make_shared<MovableFuelObject>(Vector{12, 5}, Vector{-7, 3}, 10);

    MoveMacrocommand command{object, 4};

    EXPECT_NO_THROW(command.execute());
    EXPECT_EQ(object->getLocation(), (Vector{5, 8}));
    EXPECT_EQ(object->getFuel(), 6);
}

TEST(MoveMacrocommand, DoesNotMoveOrBurnFuelWhenFuelIsNotEnough) {
    auto object = std::make_shared<MovableFuelObject>(Vector{12, 5}, Vector{-7, 3}, 3);

    MoveMacrocommand command{object, 4};

    EXPECT_THROW(command.execute(), CommandException);
    EXPECT_EQ(object->getLocation(), (Vector{12, 5}));
    EXPECT_EQ(object->getFuel(), 3);
}
