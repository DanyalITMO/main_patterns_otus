#pragma once
#include <compare>

struct Angle
{
    int direction;
    int n;

    auto operator<=>(const Angle&) const = default;
    Angle& operator+=(const Angle& other);
};

const Angle operator+(const Angle& lhs, const Angle& rhs);
