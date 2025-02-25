#pragma once

#include "../cube/cube.h"

#include "pattern_database_heuristic.h"

namespace heuristic {

class HeuristicFunction {
public:
  HeuristicFunction();
  HeuristicFunction(HeuristicFunction&& other) noexcept = default;

  HeuristicFunction &operator=(HeuristicFunction &&) = default;

  HeuristicFunction(const HeuristicFunction &) = delete;

  HeuristicFunction &operator=(const HeuristicFunction &) = delete;
  size_t GetHeuristic(const cube::Cube &cube) const;

private:
  std::vector<PatternDatabaseHeuristic> pdbs_{};
};

} // namespace heuristic