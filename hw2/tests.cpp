#include "Movement.h"
#include "Rotate.h"
#include "IMovable.h"
#include "IRotatable.h"
#include "angle.hpp"
#include <gtest/gtest.h>
#include <stdexcept>
#include <gmock/gmock.h>

using testing::Return;
using testing::Throw;

// --- Movement tests ---

class MoveMock : public IMovable
{
public:

    MOCK_METHOD(Vector, getLocation, (), (override));
    MOCK_METHOD(void, setLocation, (const Vector&), (override));
    MOCK_METHOD(Vector, getVelocity, (), (override));
};

//Для объекта, находящегося в точке (12, 5) и движущегося со скоростью (-7, 3) движение меняет положение объекта на (5, 8)
TEST(Movement, CorrectMove) {
    auto mock = std::make_unique<MoveMock>();
    auto* ptr = mock.get();
    EXPECT_CALL(*ptr, getLocation()).WillOnce(Return(Vector{12, 5}));
    EXPECT_CALL(*ptr, getVelocity()).WillOnce(Return(Vector{-7, 3}));
    EXPECT_CALL(*ptr, setLocation(Vector{5, 8})).Times(1);

    Movement movement{std::move(mock)};
    EXPECT_NO_THROW(movement.execute());
}

//Попытка сдвинуть объект, у которого невозможно прочитать положение в пространстве, приводит к ошибке
TEST(Movement, GetLocationException) {
    auto mock = std::make_unique<MoveMock>();
    auto* ptr = mock.get();
    EXPECT_CALL(*ptr, getLocation()).WillOnce(Throw(std::runtime_error("cannot read position")));

    Movement movement{std::move(mock)};
    EXPECT_THROW(movement.execute(), std::runtime_error);
}

//Попытка сдвинуть объект, у которого невозможно прочитать значение мгновенной скорости, приводит к ошибке
TEST(Movement, GetVelocityException) {
    auto mock = std::make_unique<MoveMock>();
    auto* ptr = mock.get();
    EXPECT_CALL(*ptr, getLocation()).WillOnce(Return(Vector{12, 5}));
    EXPECT_CALL(*ptr, getVelocity()).WillOnce(Throw(std::runtime_error("cannot read velocity")));

    Movement movement{std::move(mock)};
    EXPECT_THROW(movement.execute(), std::runtime_error);
}

//Попытка сдвинуть объект, у которого невозможно изменить положение в пространстве, приводит к ошибке
TEST(Movement, SetLocationException) {
    auto mock = std::make_unique<MoveMock>();
    auto* ptr = mock.get();
    EXPECT_CALL(*ptr, getLocation()).WillOnce(Return(Vector{12, 5}));
    EXPECT_CALL(*ptr, getVelocity()).WillOnce(Return(Vector{-7, 3}));
    EXPECT_CALL(*ptr, setLocation(Vector{5, 8})).WillOnce(Throw(std::runtime_error("cannot set position")));

    Movement movement{std::move(mock)};
    EXPECT_THROW(movement.execute(), std::runtime_error);
}

// --- Rotate tests ---

class RotateMock : public IRotatable
{
public:

    MOCK_METHOD(Angle, getAngle, (), (override));
    MOCK_METHOD(void, setAngle, (const Angle&), (override));
    MOCK_METHOD(Angle, getAngularVelocity, (), (override));
};

TEST(Rotate, CorrectRotate) {
    auto mock = std::make_unique<RotateMock>();
    auto* ptr = mock.get();
    EXPECT_CALL(*ptr, getAngle()).WillOnce(Return(Angle{4, 8}));
    EXPECT_CALL(*ptr, getAngularVelocity()).WillOnce(Return(Angle{1, 4}));
    EXPECT_CALL(*ptr, setAngle(Angle{3, 4})).Times(1);

    Rotate rotate{std::move(mock)};
    EXPECT_NO_THROW(rotate.execute());
}

TEST(Rotate, GetAngleException) {
    auto mock = std::make_unique<RotateMock>();
    auto* ptr = mock.get();
    EXPECT_CALL(*ptr, getAngle()).WillOnce(Throw(std::runtime_error("cannot read angle")));

    Rotate rotate{std::move(mock)};
    EXPECT_THROW(rotate.execute(), std::runtime_error);
}

TEST(Rotate, GetAngularVelocityException) {
    auto mock = std::make_unique<RotateMock>();
    auto* ptr = mock.get();
    EXPECT_CALL(*ptr, getAngle()).WillOnce(Return(Angle{0, 1}));
    EXPECT_CALL(*ptr, getAngularVelocity()).WillOnce(Throw(std::runtime_error("cannot read angular velocity")));

    Rotate rotate{std::move(mock)};
    EXPECT_THROW(rotate.execute(), std::runtime_error);
}

TEST(Rotate, SetAngleException) {
    auto mock = std::make_unique<RotateMock>();
    auto* ptr = mock.get();
    EXPECT_CALL(*ptr, getAngle()).WillOnce(Return(Angle{4, 8}));
    EXPECT_CALL(*ptr, getAngularVelocity()).WillOnce(Return(Angle{2, 4}));
    EXPECT_CALL(*ptr, setAngle(Angle{1, 1})).WillOnce(Throw(std::runtime_error("cannot set angle")));

    Rotate rotate{std::move(mock)};
    EXPECT_THROW(rotate.execute(), std::runtime_error);
}
