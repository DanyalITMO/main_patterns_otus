#pragma once

#include <cmath>
#include <vector>

bool is_wrong_value(double arg) {
  return std::isnan(arg) or std::isinf(arg);
}

constexpr static double e = 0.00001;
inline std::vector<double> solve(double a, double b, double c) {
  if (is_wrong_value(a) or is_wrong_value(b) or is_wrong_value(c))
    throw std::invalid_argument("all args must be number");
  if (std::abs(a) < e)
    throw std::invalid_argument("a cannot be zero");

  double discriminant = b*b - 4*a*c;
  if (std::abs(discriminant) < e) { return {-b/(2*a) };};
  if (discriminant < 0 ) return {};
  auto x1 = (-b + sqrt(discriminant))/(2*a);
  auto x2 = (-b - sqrt(discriminant))/(2*a);
  return {x1, x2};
}