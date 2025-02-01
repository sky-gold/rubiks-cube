#include "heuristic_function.h"
#include <iostream>
#include <stdexcept>

namespace heuristic {


HeuristicFunction::HeuristicFunction() {}

size_t HeuristicFunction::GetHeuristic(const cube::Cube &cube) const {
  auto left_edge_pattern_database_value = left_edge_pattern_database_.Get(cube);
  auto right_edge_pattern_database_value = right_edge_pattern_database_.Get(cube);
  auto position_edge_pattern_database_value = position_edge_pattern_database_.Get(cube);
  auto result = corner_pattern_database_.Get(cube);
  if (left_edge_pattern_database_value > result) {
    result = left_edge_pattern_database_value;
  }
  if (right_edge_pattern_database_value > result) {
    result = right_edge_pattern_database_value;
  }
  if (position_edge_pattern_database_value > result) {
    result = position_edge_pattern_database_value;
  }
  return result;
}

} // namespace heuristic