#include <cmath>
#include <vector>

#include "random.h"
#include "sa.h"
#include "tsp.h"

namespace tms {
class Solution;

class SolutionPool {
  const std::vector<std::vector<double>> &costMatrix;
  const bool roundtrip;
  const UINT pathSize;

  RealRandomGen randomGen;
  std::vector<std::vector<UINT>> holder;
  std::vector<UINT> unused;

public:
  SolutionPool(const std::vector<std::vector<double>> &costMatrix,
               bool roundtrip);

  std::vector<UINT> &get(UINT index);

  void reclaim(Solution sol);

  Solution create();

  Solution nearestNeighbourCH();

  double random();

  UINT getPathSize() const;

  UINT getRandomIndex(UINT start, UINT end);

  double calcCost(const std::vector<UINT> &path);
};

class Solution {
  SolutionPool *pool;
  UINT poolIndex;
  mutable double cost;

public:
  Solution(SolutionPool &pool, UINT poolIndex, double cost);

  Solution(const Solution &) = default;

  Solution &operator=(const Solution &) = default;

  bool operator==(const Solution &rhs) const;

  bool operator!=(const Solution &rhs) const;

  double getCost() const;

  std::vector<UINT> &getPath() const;

  Solution neighbour();

  void destroy();

  Solution twoOpt(UINT a, UINT b);

  Solution translate(UINT a, UINT b);

  Solution switching(UINT a, UINT b);

  friend class SolutionPool;
};

class OnRound {
  RoundCallback callback;

public:
  OnRound(RoundCallback callback) : callback(callback) {}

  void call(int iterations, int reheatCount, double temperature,
            const Solution &solution) {
    if (callback != nullptr) {
      callback(iterations, reheatCount, temperature, solution.getPath(),
               solution.getCost());
    }
  }
};

std::vector<UINT> solveTsp(int N, double T, double lambda, int reheatInterval,
                           RoundCallback roundCallback,
                           const std::vector<std::vector<double>> &costMatrix,
                           bool roundtrip) {
  SolutionPool pool(costMatrix, roundtrip);

  auto nnSol = pool.nearestNeighbourCH();

  if (N == 0 || pool.getPathSize() < 4) {
    return std::vector<UINT>(nnSol.getPath());
  }

  SAOptions<Solution, OnRound> options(N, T, lambda, reheatInterval,
                                       OnRound(roundCallback));

  SASolver<Solution, OnRound> solver(options);

  auto bestSolution = solver.solve(nnSol);

  return std::vector<UINT>(bestSolution.getPath());
}

SolutionPool::SolutionPool(const std::vector<std::vector<double>> &costMatrix,
                           bool roundtrip)
    : costMatrix(costMatrix), roundtrip(roundtrip),
      pathSize(costMatrix.size() + (roundtrip ? 1 : 0)), randomGen(0., 1.) {}

std::vector<UINT> &SolutionPool::get(UINT index) { return holder[index]; }

void SolutionPool::reclaim(Solution sol) {
  if (sol.pool == this) {
    unused.push_back(sol.poolIndex);
  }
}

Solution SolutionPool::create() {
  if (unused.empty()) {
    holder.push_back(std::vector<UINT>(pathSize));
    unused.push_back(holder.size() - 1);
  }

  auto toUse = unused.back();
  unused.pop_back();

  return Solution(*this, toUse, -1);
}

Solution SolutionPool::nearestNeighbourCH() {
  auto sol = create();
  auto &path = sol.getPath();

  auto N = costMatrix.size();
  auto toVisit = std::vector<int>(costMatrix.size(), 1);
  auto toVisitCount = N;

  auto pi = 0;

  path[pi++] = 0;
  toVisit[0] = 0;
  toVisitCount--;

  if (!roundtrip) {
    toVisit[N - 1] = 0;
    toVisitCount--;
  }

  UINT cur = 0;
  while (toVisitCount > 0) {
    bool firstIter = true;
    UINT nearest = 0;
    double minCost = -1;
    for (UINT i = 0; i < N; i++) {
      if (toVisit[i]) {
        auto cost = costMatrix[cur][i];
        if (firstIter || cost < minCost) {
          nearest = i;
          minCost = cost;
          firstIter = false;
        }
      }
    }

    path[pi++] = nearest;
    toVisit[nearest] = false;
    toVisitCount--;

    cur = nearest;
  }

  if (roundtrip) {
    path[pi++] = 0;
  }

  else {
    path[pi++] = N - 1;
  }

  return sol;
}

double SolutionPool::random() { return randomGen(); }

UINT SolutionPool::getPathSize() const { return pathSize; }

UINT SolutionPool::getRandomIndex(UINT start, UINT end) {
  return start + (UINT)std::floor(random() * (end - start));
}

double SolutionPool::calcCost(const std::vector<UINT> &path) {
  double cost = 0;
  for (UINT i = 0; i + 1 < path.size(); i++) {
    cost += costMatrix[path[i]][path[i + 1]];
  }
  return cost;
}

Solution::Solution(SolutionPool &pool, const UINT poolIndex, const double cost)
    : pool(&pool), poolIndex(poolIndex), cost(cost) {}

double Solution::getCost() const {
  if (cost == -1) {
    cost = pool->calcCost(getPath());
  }
  return cost;
}

std::vector<UINT> &Solution::getPath() const { return pool->get(poolIndex); }

Solution Solution::neighbour() {
  // ensures endpoints are not involved in manipulation
  // assumes path.length >= 4 and k >= 2
  auto k = 2; // k >= 2

  auto a = pool->getRandomIndex(1, pool->getPathSize() - k);
  auto b = a + pool->getRandomIndex(2, pool->getPathSize() - a);

  auto roll = pool->random();

  if (roll < 0.40) {
    return twoOpt(a, b);
  }

  else if (roll < (0.40 + 0.40)) {
    return translate(a, b);
  }

  else {
    return switching(a, b);
  }
}

void Solution::destroy() { pool->reclaim(*this); }

Solution Solution::twoOpt(UINT a, UINT b) {
  auto newSol = pool->create();
  auto &path = getPath();
  auto &newPath = newSol.getPath();

  UINT i = 0, j = 0;
  for (; i < a; i++, j++) {
    newPath[j] = path[i];
  }
  for (i = b - 1; i >= a; i--, j++) {
    newPath[j] = path[i];
  }
  for (i = b; i < path.size(); i++, j++) {
    newPath[j] = path[i];
  }

  return newSol;
}

Solution Solution::translate(UINT a, UINT b) {
  auto newSol = pool->create();
  auto &path = getPath();
  auto &newPath = newSol.getPath();

  UINT i = 0, j = 0;
  for (; i < a; i++, j++) {
    newPath[j] = path[i];
  }
  newPath[j++] = path[b - 1];
  for (i = a; i < b - 1; i++, j++) {
    newPath[j] = path[i];
  }
  for (i = b; i < path.size(); i++, j++) {
    newPath[j] = path[i];
  }

  return newSol;
}

Solution Solution::switching(UINT a, UINT b) {
  auto newSol = pool->create();
  auto &path = getPath();
  auto &newPath = newSol.getPath();

  UINT i = 0, j = 0;
  for (; i < a; i++, j++) {
    newPath[j] = path[i];
  }
  newPath[j++] = path[b - 1];
  for (i = a + 1; i < b - 1; i++, j++) {
    newPath[j] = path[i];
  }
  newPath[j++] = path[a];
  for (i = b; i < path.size(); i++, j++) {
    newPath[j] = path[i];
  }

  return newSol;
}

bool Solution::operator==(const Solution &rhs) const {
  return pool == rhs.pool && poolIndex == rhs.poolIndex;
}

bool Solution::operator!=(const Solution &rhs) const { return !(rhs == *this); }
}
