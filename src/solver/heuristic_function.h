#pragma once

#include "../cube/cube.h"
namespace solver {

class HeuristicFunction {
public:
  HeuristicFunction();
  size_t GetHeuristic(const cube::Cube &cube) const;

  size_t GetHeuristic(const cube::Cube &start, const cube::Cube &finish) const;

private:
  std::array<std::array<size_t, cube::CORNER_CUBIE_CNT>, cube::CORNER_CUBIE_CNT>
      corner_cubie_distance{};
  std::array<std::array<size_t, cube::EDGE_CUBIE_CNT>, cube::EDGE_CUBIE_CNT>
      edge_cubie_distance{};
};

} // namespace solver