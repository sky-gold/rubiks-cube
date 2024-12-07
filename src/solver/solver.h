#pragma once

#include "heuristic_function.h"

#include <optional>

namespace solver {

class Solver {
public:
  Solver(HeuristicFunction h_func, size_t threshold = 20);

  std::optional<std::vector<cube::Move>> Solve(const cube::Cube &start);

private:
  const size_t threshold_;
  const HeuristicFunction h_func_;
};

} // namespace solver