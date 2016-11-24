#pragma once

#include <random>

namespace tms {
class IntRandomGen {
  std::mt19937 engine{std::random_device()()};
  std::uniform_int_distribution<int> dist;

public:
  IntRandomGen(int a, int b) : dist(a, b) {}

  int operator()() { return dist(engine); }
};

class RealRandomGen {
  std::mt19937 engine{std::random_device()()};
  std::uniform_real_distribution<double> dist;

public:
  RealRandomGen(double a, double b) : dist(a, b) {}

  double operator()() { return dist(engine); }
};
}
