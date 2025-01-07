#pragma once

#include "../cube/cube.h"
#include "cubie_distance.h"

namespace solver {

class HeuristicFunction {
public:
  HeuristicFunction();
  size_t GetHeuristic(const cube::Cube &cube) const;

  size_t GetHeuristic(const cube::Cube &start, const cube::Cube &finish) const;

private:
  const CubieDistance cubie_distance_{};
};

} // namespace solver