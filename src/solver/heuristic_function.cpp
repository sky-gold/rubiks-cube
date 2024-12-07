#include "heuristic_function.h"

namespace solver {

size_t HeuristicFunction::GetHeuristic(const cube::Cube &cube) const {
  size_t misplaced_cnt = 0;
  const auto &edge_cubies = cube.GetEdgeCubies();
  const auto &corner_cubies = cube.GetCornerCubies();
  for (size_t i = 0; i < edge_cubies.size(); ++i) {
    if (edge_cubies[i] != cube::GetEdgeCubie(i, 0)) {
      ++misplaced_cnt;
    }
  }
  for (size_t i = 0; i < corner_cubies.size(); ++i) {
    if (corner_cubies[i] != cube::GetCornerCubie(i, 0)) {
      ++misplaced_cnt;
    }
  }
  return (misplaced_cnt + 7) / 8;
}

} // namespace solver