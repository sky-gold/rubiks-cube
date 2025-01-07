#include "heuristic_function.h"
#include <iostream>
#include <stdexcept>

namespace solver {


HeuristicFunction::HeuristicFunction() {}

size_t HeuristicFunction::GetHeuristic(const cube::Cube &cube) const {
  return GetHeuristic(cube, cube::Cube());
}

size_t HeuristicFunction::GetHeuristic(const cube::Cube &start,
                                       const cube::Cube &finish) const {
  return cubie_distance_.GetHeuristic(start, finish);
}

} // namespace solver