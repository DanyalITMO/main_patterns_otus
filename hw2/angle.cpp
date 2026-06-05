#include "angle.hpp"
#include <numeric>

Angle& Angle::operator+=(const Angle& other) {
    const int common_denominator = n * other.n;
    const int common_numerator = direction * other.n + other.direction * n;
    const int gcd = std::gcd(common_numerator, common_denominator);
    direction = common_numerator / gcd;
    n = common_denominator / gcd;
    return *this;
}

const Angle operator+(const Angle& lhs, const Angle& rhs) {
    Angle ret(lhs);
    ret += rhs;
    return ret;
}
