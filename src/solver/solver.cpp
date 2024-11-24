#include "solver.h"

#include <algorithm>
#include <unordered_map>

namespace solver {

namespace {

std::optional<std::vector<cube::Move>> bfs(const cube::Cube& start, size_t threshold) {
  std::unordered_map<cube::Cube, std::pair<size_t, cube::Move>> distance;
  std::vector<cube::Cube> q = {start};
  distance[start] = {0, 0};
  for (int i = 0; i < q.size(); ++i) {
    const auto cur = q[i];
    if (distance[cur].first == threshold) {
      continue;
    }
    if (cur.IsSolved()) {
      break;
    }
    for (int face = 0; face < 6; ++face) {
      for (int rotation = 1; rotation <= 3; ++rotation) {
        auto move = cube::GetMove(rotation, face);
        auto neighbor = cur;
        neighbor.Turn(move);
        if (distance.count(neighbor)) {
          continue;
        }
        q.push_back(neighbor);
        distance[neighbor] = {distance[cur].first + 1, move};
      }
    }
  }
  if (!distance.count(cube::Cube())) {
    return std::nullopt;
  }
  std::vector<cube::Move> ans;
  auto cube = cube::Cube();
  while (cube != start) {
    auto move = distance[cube].second;
    ans.push_back(move);
    auto anti_move = cube::GetMove(4 - cube::getRotation(move), cube::GetFace(move));
    cube.Turn(anti_move);
  }
  std::reverse(ans.begin(), ans.end());
  return ans;
}

} // namespace

Solver::Solver(size_t threshold) : threshold_(threshold){};

std::optional<std::vector<cube::Move>> Solver::Solve(const cube::Cube& start) {
    return bfs(start, threshold_);
}

} // namespace solver