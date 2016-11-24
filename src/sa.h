#pragma once

#include <cmath>
#include <functional>
#include <vector>

#include "random.h"

namespace tms {
template <typename SolutionType, typename OnRoundType> class SAOptions {
  const int N;              // Number of iterations
  const double T;           // Initial temperature
  const double lambda;      // annealing parameter
  const int reheatInterval; // Iterations before resetting temperature

  OnRoundType onRound; // Function to call after every iteration

public:
  SAOptions(int N, double T, double lambda, int reheatInterval,
            OnRoundType onRound)
      : N(N), T(T), lambda(lambda), reheatInterval(reheatInterval),
        onRound(onRound) {}

  int getN() const { return N; }

  double getT() const { return T; }

  double getLambda() const { return lambda; }

  int getReheatInterval() const { return reheatInterval; }

  void callOnRound(int iterations, int reheatCount, double temperature,
                   const SolutionType &solution) {
    onRound.call(iterations, reheatCount, temperature, solution);
  }
};

template <typename SolutionType, typename OnRoundType> class SASolver {
  SAOptions<SolutionType, OnRoundType> options;
  RealRandomGen random = RealRandomGen(0.0, 1.0);

  bool metropolis(double c1, double c2, double T) {
    return random() <= std::exp(-(c1 - c2) / T);
  }

  double anneal(double T) { return T * options.getLambda(); }

public:
  SASolver(SAOptions<SolutionType, OnRoundType> options) : options(options) {}

  SolutionType solve(SolutionType initSolution) {

    double T = options.getT();
    int reheatCount = -1;

    auto curSol = initSolution;
    auto bestSol = initSolution;

    for (int i = 0; i < options.getN(); i++) {
      if (i % options.getReheatInterval() == 0) {
        reheatCount++;
        T = options.getT();
      }

      SolutionType newSol = curSol.neighbour();

      if (newSol.getCost() < curSol.getCost()) {
        if (curSol != bestSol) {
          curSol.destroy();
        }
        curSol = newSol;

        if (newSol.getCost() < bestSol.getCost()) {
          bestSol.destroy();
          bestSol = newSol;
        }

      } else if (metropolis(newSol.getCost(), curSol.getCost(), T)) {
        if (curSol != bestSol) {
          curSol.destroy();
        }
        curSol = newSol;
      } else {
        newSol.destroy();
      }

      T = anneal(T);

      options.callOnRound(i, reheatCount, T, curSol);
    }

    return bestSol;
  }
};
}
