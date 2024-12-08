#include "heuristic_function.h"
#include <iostream>
#include <stdexcept>
#include <unordered_map>

namespace solver {

namespace {

size_t find_distance_corner_cubie(uint8_t start, uint8_t end) {
  std::unordered_map<uint8_t, size_t> distance;
  distance[start] = 0;
  std::vector<uint8_t> q = {start};
  size_t i = 0;
  while (i < q.size()) {
    if (distance.count(end)) {
      break;
    }
    uint8_t cur = q[i];
    ++i;
    for (int face = 0; face < 6; ++face) {
      for (int rotation = 1; rotation <= 3; ++rotation) {
        auto move = cube::GetMove(rotation, face);
        auto neighbor = cube::corner_permutation[move][cur];
        if (distance.count(neighbor)) {
          continue;
        }
        distance[neighbor] = distance[cur] + 1;
        q.push_back(neighbor);
      }
    }
  }
  if (!distance.count(end)) {
    return 0;
  }
  return distance[end];
}

size_t find_distance_edge_cubie(uint8_t start, uint8_t end) {
  std::unordered_map<uint8_t, size_t> distance;
  distance[start] = 0;
  std::vector<uint8_t> q = {start};
  size_t i = 0;
  while (i < q.size()) {
    uint8_t cur = q[i];
    ++i;
    if (distance.count(end)) {
      break;
    }
    for (int face = 0; face < 6; ++face) {
      for (int rotation = 1; rotation <= 3; ++rotation) {
        auto move = cube::GetMove(rotation, face);
        auto neighbor = cube::edge_permutation[move][cur];
        if (distance.count(neighbor)) {
          continue;
        }
        distance[neighbor] = distance[cur] + 1;
        q.push_back(neighbor);
      }
    }
  }

  if (!distance.count(end)) {
    return 0;
  }
  return distance[end];
}

} // namespace

HeuristicFunction::HeuristicFunction() {
  std::vector<uint8_t> corner_cubies;
  for (size_t pos = 0; pos < cube::CORNER_INDEX_CNT; ++pos) {
    for (size_t rot = 0; rot < 3; ++rot) {
      corner_cubies.push_back(cube::GetCornerCubie(pos, rot));
    }
  }
  for (auto start : corner_cubies) {
    for (auto finish : corner_cubies) {
      corner_cubie_distance[start][finish] =
          find_distance_corner_cubie(start, finish);
    }
  }
  std::vector<uint8_t> edge_cubies;
  for (size_t pos = 0; pos < cube::EDGE_INDEX_CNT; ++pos) {
    for (size_t flip = 0; flip < 2; ++flip) {
      edge_cubies.push_back(cube::GetEdgeCubie(pos, flip));
    }
  }
  for (auto start : edge_cubies) {
    for (auto finish : edge_cubies) {
      edge_cubie_distance[start][finish] =
          find_distance_edge_cubie(start, finish);
    }
  }
}

size_t HeuristicFunction::GetHeuristic(const cube::Cube &cube) const {
  size_t misplaced_cnt = 0;
  const auto &edge_cubies = cube.GetEdgeCubies();
  const auto &corner_cubies = cube.GetCornerCubies();
  for (size_t i = 0; i < edge_cubies.size(); ++i) {
    misplaced_cnt +=
        edge_cubie_distance[cube::GetEdgeCubie(i, 0)][edge_cubies[i]];
  }
  for (size_t i = 0; i < corner_cubies.size(); ++i) {
    misplaced_cnt +=
        corner_cubie_distance[cube::GetCornerCubie(i, 0)][corner_cubies[i]];
  }
  return (misplaced_cnt + 7) / 8;
}

size_t HeuristicFunction::GetHeuristic(const cube::Cube &start,
                                       const cube::Cube &finish) const {
  size_t misplaced_cnt = 0;
  const auto &start_edge_cubies = start.GetEdgeCubies();
  const auto &start_corner_cubies = start.GetCornerCubies();
  const auto &finish_edge_cubies = finish.GetEdgeCubies();
  const auto &finish_corner_cubies = finish.GetCornerCubies();
  for (size_t i = 0; i < cube::EDGE_INDEX_CNT; ++i) {
    misplaced_cnt +=
        edge_cubie_distance[start_edge_cubies[i]][finish_edge_cubies[i]];
  }
  for (size_t i = 0; i < cube::CORNER_INDEX_CNT; ++i) {
    misplaced_cnt +=
        corner_cubie_distance[start_corner_cubies[i]][finish_corner_cubies[i]];
  }
  return (misplaced_cnt + 7) / 8;
}

} // namespace solver