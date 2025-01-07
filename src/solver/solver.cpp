#include "solver.h"

#include <limits>
#include <iostream>

namespace solver {

namespace {

constexpr size_t INF = std::numeric_limits<size_t>::max();

}

Solver::Solver(HeuristicFunction h_func, size_t threshold)
    : threshold_(threshold), h_func_(std::move(h_func)){};

std::optional<std::vector<cube::Move>> Solver::Solve(cube::Cube start) {
  size_t bound = h_func_.GetHeuristic(start);
  std::vector<cube::Move> path;
  std::optional<std::vector<cube::Move>> result;
  while (true) {
    bound = Search(start, path, 0, result, bound);
    if (result.has_value()) {
      return result;
    }
    if (bound == INF) {
      return std::nullopt;
    }
  }
}

size_t Solver::Search(cube::Cube & cube, std::vector<cube::Move>& path, size_t distance, std::optional<std::vector<cube::Move>>& result, size_t bound) {
  if (result.has_value()) {
    return 0;
  }
  if (cube.IsSolved()) {
    result = path;
    return 0;
  }
  size_t f = distance + h_func_.GetHeuristic(cube);
  if (f > bound) {
    return f;
  }
  size_t min = INF;
   for (int face = 0; face < 6; ++face) {
      for (int rotation = 1; rotation <= 3; ++rotation) {
        auto move = cube::GetMove(rotation, face);
        path.push_back(move);
        cube.Turn(move);
        size_t node_bound = Search(cube, path, distance + 1, result, bound);
        if (result.has_value()) {
          return 0;
        }
        if (min > node_bound) {
          min = node_bound;
        }
        cube.Turn(cube::GetAntiMove(move));
        path.pop_back();
      }
   }
   return min;
}


} // namespace solver