#pragma once

#include "../heuristic/heuristic_function.h"

#include <optional>

namespace solver {

class Solver {
public:
  Solver(heuristic::HeuristicFunction&& h_func);

  std::optional<std::vector<cube::Move>> Solve(cube::Cube start);

private:
  size_t Search(cube::Cube &cube, std::vector<cube::Move> &path,
                size_t distance, std::optional<std::vector<cube::Move>> &result,
                size_t bound);

  const heuristic::HeuristicFunction h_func_;

};

} // namespace solver