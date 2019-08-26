#pragma once

#include <random>

namespace tsp {
class RealRandomGen {
  std::mt19937 engine{std::random_device()()};
  std::uniform_real_distribution<double> dist;

public:
  RealRandomGen(double a, double b) : dist(a, b) {}

  double operator()() { return dist(engine); }
};
} // namespace tsp
