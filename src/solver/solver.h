#pragma once

#include "heuristic_function.h"

#include <optional>

namespace solver {

class Solver {
public:
  Solver(HeuristicFunction h_func, size_t threshold = 20);

  std::optional<std::vector<cube::Move>> Solve(cube::Cube start);

private:
  size_t Search(cube::Cube &cube, std::vector<cube::Move> &path,
                size_t distance, std::optional<std::vector<cube::Move>> &result,
                size_t bound);

  const size_t threshold_;
  const HeuristicFunction h_func_;
};

} // namespace solver