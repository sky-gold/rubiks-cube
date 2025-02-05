#include "solver.h"

#include <iostream>
#include <limits>

namespace solver {

namespace {

constexpr size_t INF = std::numeric_limits<size_t>::max();

} // namespace

Solver::Solver(heuristic::HeuristicFunction&& h_func)
    : h_func_(std::move(h_func)){};

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

size_t Solver::Search(cube::Cube &cube, std::vector<cube::Move> &path,
                      size_t distance,
                      std::optional<std::vector<cube::Move>> &result,
                      size_t bound) {
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
  uint8_t repeated_face = 7;
  if (!path.empty()) {
    repeated_face = cube::GetFace(path.back());
  }
  uint8_t repeated_opposite_face = 7;
  if (path.size() >= 2 &&
      cube::GetFace(path[path.size() - 2]) ==
          cube::GetOppositeFace(cube::GetFace(path[path.size() - 1]))) {
    repeated_opposite_face = cube::GetFace(path[path.size() - 2]);
  }
  if (!path.empty() &&
      cube::GetOppositeFace(cube::GetFace(path[path.size() - 1])) <
          cube::GetFace(path[path.size() - 1])) {
    repeated_opposite_face =
        cube::GetOppositeFace(cube::GetFace(path[path.size() - 1]));
  }
  for (int face = 0; face < 6; ++face) {
    if (face == repeated_face || face == repeated_opposite_face) {
      continue;
    }
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