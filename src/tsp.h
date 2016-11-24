#pragma once

#include <vector>

namespace tms {
typedef unsigned long UINT;

typedef void (*RoundCallback)(int iterations, int reheatCount,
                              double temperature, const std::vector<UINT> &path,
                              double cost);

std::vector<UINT> solveTsp(int N, double T, double lambda, int reheatInterval,
                           RoundCallback roundCallback,
                           const std::vector<std::vector<double>> &costMatrix,
                           bool roundtrip);
}
