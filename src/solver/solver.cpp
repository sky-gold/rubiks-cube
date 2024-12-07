#include "solver.h"

#include <algorithm>
#include <set>
#include <unordered_map>
#include <unordered_set>

namespace solver {

Solver::Solver(HeuristicFunction h_func, size_t threshold)
    : threshold_(threshold), h_func_(std::move(h_func)){};

std::optional<std::vector<cube::Move>> Solver::Solve(const cube::Cube &start) {
  std::set<std::pair<size_t, cube::Cube>> f_set;
  std::unordered_map<cube::Cube, std::pair<size_t, cube::Move>> distance;
  distance[start] = {0, 0};
  f_set.insert({h_func_.GetHeuristic(start), start});
  size_t iter_count = 0;
  while (!f_set.empty()) {
    ++iter_count;
    auto it = f_set.begin();
    auto [f_value, cur] = *it;
    f_set.erase(it);
    if (cur.IsSolved()) {
      break;
    }
    const auto cur_distance = distance[cur].first;
    if (cur_distance == threshold_) {
      continue;
    }
    for (int face = 0; face < 6; ++face) {
      for (int rotation = 1; rotation <= 3; ++rotation) {
        auto move = cube::GetMove(rotation, face);
        auto neighbor = cur;
        neighbor.Turn(move);
        if (distance.count(neighbor)) {
          continue;
        }
        const auto neighbor_distance = cur_distance + 1;
        distance[neighbor] = {neighbor_distance, move};
        f_set.insert(
            {neighbor_distance + h_func_.GetHeuristic(neighbor), neighbor});
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
    auto anti_move =
        cube::GetMove(4 - cube::getRotation(move), cube::GetFace(move));
    cube.Turn(anti_move);
  }
  std::reverse(ans.begin(), ans.end());
  return ans;
}

} // namespace solver