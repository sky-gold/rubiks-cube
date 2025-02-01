#include "heuristic_function.h"
#include <iostream>
#include <stdexcept>

namespace heuristic {


HeuristicFunction::HeuristicFunction() {}

size_t HeuristicFunction::GetHeuristic(const cube::Cube &cube) const {
  auto cubie_distance_value = cubie_distance_.GetHeuristic(cube, cube::Cube());
  auto corner_pattern_database_value = corner_pattern_database_.Get(cube);
  auto left_edge_pattern_database_value = left_edge_pattern_database_.Get(cube);
  auto right_edge_pattern_database_value = right_edge_pattern_database_.Get(cube);
  auto result = cubie_distance_value;
  if (corner_pattern_database_value > result) {
    result = corner_pattern_database_value;
  }
  if (left_edge_pattern_database_value > result) {
    result = left_edge_pattern_database_value;
  }
  if (right_edge_pattern_database_value > result) {
    result = right_edge_pattern_database_value;
  }
  return result;
}

} // namespace heuristic