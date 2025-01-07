#include <gtest/gtest.h>
#include <map>
#include <unordered_map>
#include <vector>

#include "cube/cube.h"

namespace {

std::vector<int> bfs(const cube::Cube flat_cube, int limit) {
  std::unordered_map<cube::Cube, size_t> distance;
  std::vector<cube::Cube> q = {flat_cube};
  distance[flat_cube] = 0;
  for (int i = 0; i < q.size(); ++i) {
    const auto cur = q[i];
    if (distance[cur] == limit - 1) {
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
        q.push_back(neighbor);
        distance[neighbor] = distance[cur] + 1;
      }
    }
  }
  std::vector<int> ans(limit);
  for (auto to : distance) {
    ++ans[to.second];
  }
  return ans;
}

} // namespace

TEST(PositionsCount, TestBFS) {
  std::vector<int> positions_count = bfs(cube::Cube(), 6);
  std::vector<int> expected_count = {
      1,    18,    243,
      3240, 43239, 574908}; //, 7618438 Values from https://www.cube20.org/
  EXPECT_EQ(positions_count, expected_count);
}
