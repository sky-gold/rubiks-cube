#include "heuristic_function.h"
#include <iostream>
#include <stdexcept>

#include "corner_coordinate_calculator.h"
#include "left_edge_coordinate_calculator.h"
#include "right_edge_coordinate_calculator.h"


namespace heuristic {

HeuristicFunction::HeuristicFunction() {
  pdbs_.emplace_back(PatternDatabaseHeuristic(0, 4, std::make_unique<LeftEdgeCoordinateCalculator>(8), "l8epdb.bin"));
  pdbs_.emplace_back(PatternDatabaseHeuristic(0, 4, std::make_unique<RightEdgeCoordinateCalculator>(8), "r8epdb.bin", 12));
  pdbs_.emplace_back(PatternDatabaseHeuristic(0, 4, std::make_unique<CornerCoordinateCalculator>(), "cpdb.bin", 11));
}

size_t HeuristicFunction::GetHeuristic(const cube::Cube &cube) const {
  if (pdbs_.empty()) {
    return 0;
  }
  size_t result = pdbs_.front().GetHeuristic(cube);
  for (size_t i = 1; i < pdbs_.size(); ++i) {
    if (pdbs_[i].GetMaxValue() >= result) {
      size_t new_result = pdbs_[i].GetHeuristic(cube);
      if (new_result > result) {
        result = new_result;
      }
    }
  }
  return result;
}

} // namespace heuristic