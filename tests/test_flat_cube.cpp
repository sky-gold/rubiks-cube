#include <gtest/gtest.h>
#include <map>
#include <unordered_map>
#include <vector>

#include "cube/simple_flat_cube.h"

std::vector<int> bfs(const cube::SimpleFlatCube flat_cube, int limit) {
  std::unordered_map<cube::SimpleFlatCube, size_t> distance;
  std::vector<cube::SimpleFlatCube> q = {flat_cube};
  distance[flat_cube] = 0;
  for (int i = 0; i < q.size(); ++i) {
    const auto cur = q[i];
    if (distance[cur] == limit - 1) {
      continue;
    }
    for (int face = 0; face < 6; ++face) {
      for (int rotation = 1; rotation <= 3; ++rotation) {
        for (int index = 0; index < flat_cube.GetSize() / 2; ++index) {
          auto move = cube::GetMove(index, rotation, face);
          auto neighbor = cur;
          neighbor.Turn(move);
          if (distance.count(neighbor)) {
            continue;
          }
          q.push_back(neighbor);
          distance[neighbor] = distance[cur] + 1;
        }
      }
    }
  }
  std::vector<int> ans(limit);
  for (auto to : distance) {
    ++ans[to.second];
  }
  return ans;
}

TEST(FlatCubeTest, TestCountOfPositions2x2x2) {
  std::vector<int> positions_count = bfs(cube::SimpleFlatCube(2), 4);
  std::vector<int> expected_count = {
      1, 9, 54, 321}; // Values from https://en.wikipedia.org/wiki/Pocket_Cube
  EXPECT_EQ(positions_count, expected_count);
}

TEST(FlatCubeTest, TestCountOfPositions3x3x3) {
  std::vector<int> positions_count = bfs(cube::SimpleFlatCube(3), 5);
  std::vector<int> expected_count = {
      1, 18, 243, 3240, 43239}; // , 574908 Values from https://www.cube20.org/
  EXPECT_EQ(positions_count, expected_count);
}

TEST(FlatCubeTest, TestCountOfPositions4x4x4) {
  std::vector<int> positions_count = bfs(cube::SimpleFlatCube(4), 3);
  std::vector<int> expected_count = {
      1, 36, 1023}; // Not found public data, maybe incorrect
  EXPECT_EQ(positions_count, expected_count);
}

TEST(FlatCubeTest, TestCountOfPositions5x5x5) {
  std::vector<int> positions_count = bfs(cube::SimpleFlatCube(5), 5);
  std::vector<int> expected_count = {
      1, 36, 1026, 28812, 806988}; // Not found public data, maybe incorrect
  EXPECT_EQ(positions_count, expected_count);
}
