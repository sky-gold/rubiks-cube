#pragma once

#include "../heuristic/heuristic_function.h"

#include <atomic>
#include <mutex>
#include <optional>

namespace solver {

class Solver {
public:
  Solver(heuristic::HeuristicFunction &&h_func);

  std::optional<std::vector<cube::Move>> Solve(cube::Cube start);

private:
  size_t ParallelSearch(const cube::Cube &start, size_t bound);
  size_t Search(cube::Cube &cube, std::vector<cube::Move> &path,
                size_t distance, size_t bound);

  const heuristic::HeuristicFunction h_func_;

  std::optional<std::vector<cube::Move>> shared_result_;
  std::mutex result_mutex_;
  std::atomic<bool> found_solution_;
  size_t num_threads_ = 4;
};

} // namespace solver