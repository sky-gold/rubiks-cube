#include "solver.h"

#include <algorithm>
#include <iostream>
#include <set>
#include <unordered_map>
#include <unordered_set>

namespace solver {

namespace {

std::vector<cube::Move>
find_path(const std::unordered_map<cube::Cube, std::pair<size_t, cube::Move>>
              &first_distance,
          const std::unordered_map<cube::Cube, std::pair<size_t, cube::Move>>
              &second_distance,
          const cube::Cube &start, const cube::Cube &end) {
  cube::Cube min_first_cube;
  cube::Cube min_second_cube;
  size_t min_distance = 21;
  cube::Move bridge_move;
  for (auto to : first_distance) {
    cube::Cube first_cube = to.first;
    for (int face = 0; face < 6; ++face) {
      for (int rotation = 1; rotation <= 3; ++rotation) {
        auto move = cube::GetMove(rotation, face);
        cube::Cube second_cube = first_cube;
        second_cube.Turn(move);
        if (second_distance.count(second_cube)) {
          const auto distance = first_distance.at(first_cube).first + 1 +
                                second_distance.at(second_cube).first;
          if (distance < min_distance) {
            min_distance = distance;
            min_first_cube = first_cube;
            min_second_cube = second_cube;
            bridge_move = move;
          }
        }
      }
    }
  }
  // std::cout << "min_first_cube=" << min_first_cube.GetHash() << std::endl;
  // std::cout << "min_second_cube=" << min_second_cube.GetHash() << std::endl;
  // std::cout << "min_distance=" << min_distance << std::endl;
  std::vector<cube::Move> ans;
  auto cube = min_first_cube;
  while (cube != start) {
    auto move = first_distance.at(cube).second;
    ans.push_back(move);
    auto anti_move =
        cube::GetMove(4 - cube::getRotation(move), cube::GetFace(move));
    cube.Turn(anti_move);
  }
  std::reverse(ans.begin(), ans.end());
  ans.push_back(bridge_move);
  cube = min_second_cube;
  while (cube != end) {
    auto move = second_distance.at(cube).second;
    auto anti_move =
        cube::GetMove(4 - cube::getRotation(move), cube::GetFace(move));
    ans.push_back(anti_move);
    cube.Turn(anti_move);
  }
  return ans;
}

bool a_star(
    const HeuristicFunction &h_func, const cube::Cube &end,
    std::set<std::pair<size_t, cube::Cube>> &f_set,
    std::unordered_map<cube::Cube, std::pair<size_t, cube::Move>> &distance,
    const std::unordered_map<cube::Cube, std::pair<size_t, cube::Move>>
        &other_distance,
    size_t limit) {

  while (!f_set.empty()) {
    if (limit == 0) {
      break;
    }
    --limit;
    auto it = f_set.begin();
    auto [f_value, cur] = *it;
    f_set.erase(it);
    // std::cout << "cur=" << cur.GetHash() << std::endl;
    // std::cout << "distance[cur]=" << distance.at(cur).first << std::endl;
    if (other_distance.count(cur)) {
      // std::cout << "other_distance[cur]=" << other_distance.at(cur).first <<
      // std::endl;
      return true;
    }
    const auto cur_distance = distance[cur].first;
    for (int face = 0; face < 6; ++face) {
      for (int rotation = 1; rotation <= 3; ++rotation) {
        auto move = cube::GetMove(rotation, face);
        auto neighbor = cur;
        neighbor.Turn(move);
        // std::cout << "move=" << cube::MoveToString(move) << std::endl;
        // std::cout << "neighbor=" << neighbor.GetHash() << std::endl;
        if (distance.count(neighbor) &&
            distance[neighbor].first <= distance[cur].first + 1) {
          // std::cout << "continue" << std::endl;
          continue;
        }
        const auto neighbor_distance = cur_distance + 1;
        distance[neighbor] = {neighbor_distance, move};
        f_set.insert(
            {neighbor_distance + h_func.GetHeuristic(neighbor, end), neighbor});
        // std::cout << "distance[neighbor]=" << distance.at(neighbor).first <<
        // std::endl;
        if (other_distance.count(neighbor)) {
          // std::cout << "other_distance[neighbor]=" <<
          // other_distance.at(neighbor).first << std::endl;
          return true;
        }
      }
    }
  }
  return false;
}
} // namespace

Solver::Solver(HeuristicFunction h_func, size_t threshold)
    : threshold_(threshold), h_func_(std::move(h_func)){};

std::optional<std::vector<cube::Move>> Solver::Solve(const cube::Cube &start) {
  if (start.IsSolved()) {
    return std::vector<cube::Move>{};
  }
  const cube::Cube end = cube::Cube();

  std::set<std::pair<size_t, cube::Cube>> first_f_set;
  std::unordered_map<cube::Cube, std::pair<size_t, cube::Move>> first_distance;
  first_distance[start] = {0, 0};
  first_f_set.insert({h_func_.GetHeuristic(start, end), start});

  std::set<std::pair<size_t, cube::Cube>> second_f_set;
  std::unordered_map<cube::Cube, std::pair<size_t, cube::Move>> second_distance;
  second_distance[end] = {0, 0};
  second_f_set.insert({h_func_.GetHeuristic(end, start), end});

  const size_t limit = 1;

  while (true) {
    // std::cout << "---FIRST---" << std::endl;
    if (a_star(h_func_, end, first_f_set, first_distance, second_distance,
               limit)) {
      break;
    }
    // std::cout << "---SECOND---" << std::endl;
    if (a_star(h_func_, start, second_f_set, second_distance, first_distance,
               limit)) {
      break;
    }
  }
  return find_path(first_distance, second_distance, start, end);
}

} // namespace solver