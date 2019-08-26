#pragma once

#include <vector>

namespace tsp {

typedef std::vector<uint32_t> path_type;

typedef void (*RoundCallback)(uint32_t iterations, uint32_t reheatCount,
                              double temperature, const path_type &path,
                              double cost);

path_type solveTsp(uint32_t N, double T, double lambda, uint32_t reheatInterval,
                   RoundCallback roundCallback,
                   const std::vector<std::vector<double>> &costMatrix,
                   bool roundtrip);
} // namespace tsp
