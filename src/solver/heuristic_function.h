#pragma once

#include "../cube/cube.h"
#include "cubie_distance.h"
#include "corner_pattern_database.h"

namespace solver {

class HeuristicFunction {
public:
  HeuristicFunction();
  size_t GetHeuristic(const cube::Cube &cube) const;
private:

  const CornerPatternDatabase corner_pattern_database_{};
  const CubieDistance cubie_distance_{};
};

} // namespace solver