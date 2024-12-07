#pragma once

#include "../cube/cube.h"

namespace solver {

class HeuristicFunction {
public:
  size_t GetHeuristic(const cube::Cube &cube) const;
};

} // namespace solver