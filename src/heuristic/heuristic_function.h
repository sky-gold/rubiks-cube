#pragma once

#include "../cube/cube.h"
#include "corner_pattern_database.h"
#include "left_edge_pattern_database.h"
#include "right_edge_pattern_database.h"
#include "position_edge_pattern_database.h"

namespace heuristic {

class HeuristicFunction {
public:
  HeuristicFunction();
  size_t GetHeuristic(const cube::Cube &cube) const;
private:
  const LeftEdgePatternDatabase left_edge_pattern_database_{};
  const RightEdgePatternDatabase right_edge_pattern_database_{};
  const CornerPatternDatabase corner_pattern_database_{};
  const PositionEdgePatternDatabase position_edge_pattern_database_{};
};

} // namespace heuristic