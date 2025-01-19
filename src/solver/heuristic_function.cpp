#include "heuristic_function.h"
#include <iostream>
#include <stdexcept>

namespace solver {


HeuristicFunction::HeuristicFunction() {}

size_t HeuristicFunction::GetHeuristic(const cube::Cube &cube) const {
  auto cubie_distance_value = cubie_distance_.GetHeuristic(cube, cube::Cube());
  auto corner_pattern_database_value = corner_pattern_database_.Get(cube);
  return std::max(cubie_distance_value, corner_pattern_database_value);
}

} // namespace solver