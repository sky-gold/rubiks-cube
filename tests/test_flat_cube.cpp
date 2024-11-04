#include <gtest/gtest.h>
#include <map>
#include <vector>

#include "cube/flat_cube.h"

template <size_t N>
std::vector<int> bfs(const FlatCube<N> flat_cube, int limit) {
  std::map<std::array<std::array<uint8_t, N * N>, FACES_COUNT>, size_t>
      distance;
  std::vector<FlatCube<N>> q = {flat_cube};
  distance[flat_cube.GetCube()] = 0;
  for (int i = 0; i < q.size(); ++i) {
    const auto cur = q[i];
    if (distance[cur.GetCube()] == limit - 1) {
      continue;
    }
    for (int face = 0; face < 6; ++face) {
      for (int rotation = 1; rotation <= 3; ++rotation) {
        auto move = MoveUtils::GetMove(0, rotation, face);
        auto neighbor = cur;
        neighbor.Turn(move);
        if (distance.count(neighbor.GetCube())) {
          continue;
        }
        q.push_back(neighbor);
        distance[neighbor.GetCube()] = distance[cur.GetCube()] + 1;
      }
    }
  }
  std::vector<int> ans(limit);
  for (auto to : distance) {
    ++ans[to.second];
  }
  return ans;
}

TEST(FlatCubeTest, TestCountOfPositions3x3x3) {
  std::vector<int> positions_count = bfs(FlatCube<3>(), 5);
  std::vector<int> expected_count = {
      1, 18, 243, 3240, 43239}; // Values from https://www.cube20.org/
  EXPECT_EQ(positions_count, expected_count);
}