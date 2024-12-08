#include <gtest/gtest.h>

#include "solver/heuristic_function.h"
#include "solver/solver.h"

#include <random>

namespace {

void TurnMoves(cube::Cube &cube, const std::vector<cube::Move> &moves) {
  for (auto move : moves) {
    cube.Turn(move);
  }
}

cube::Cube CubeFromMovesString(const std::string &moves_string) {
  auto cube = cube::Cube();
  TurnMoves(cube, cube::GetMovesFromString(moves_string));
  return cube;
}

} // namespace

TEST(Solver, IsLowerThanActualDistance) {
  std::vector<std::string> moves_strings = {
      "B2L1B2R3F3U3B3L1D3F3L1U1L2B2L3D2B2D2R2B2",
      "R1U2R1D2R2B2L3D3B3F1U1B3R3U2L3D1R2F3U2L2",
      "D2R2F2D2F2D2R3F2D3L2R1B1L3F1U1R3B1F2R2F3",
      "D3F3U1B2R2F1R3U2B3L1D1F1R1D2R2L2D3R2F2D3",
      "U2R2F2D3U1F2U2B1U1B3R1U3F1L1B1R3F1L2D3B1",
      "B1D3L3F3L1F1B1U3D2F3R2B3U1F2R3L1U2R2F2B2",
      "U2L3U2F2L3R1D2L2B3D2L1F3R3U3L1U2F3D3R1B1",
      "F3L1B2R1U3B3L1U2D3F1L3R2U2D2B2R2D1R2L2F2",
      "U2R2D2B1U2B3F1D3B3R3D1U2B2F2R3D3B1U3F3R2"};
  solver::HeuristicFunction heuristic_function;
  size_t diff_sum = 0;
  size_t cnt = 0;
  for (size_t distance = 1; distance <= 20; ++distance) {
    for (auto moves_string : moves_strings) {
      const auto cube =
          CubeFromMovesString(moves_string.substr(0, distance * 2));
      const auto heuristic_value = heuristic_function.GetHeuristic(cube);
      //   std::cout << "heuristic_value=" << heuristic_value << " distance=" <<
      //   distance << std::endl;
      EXPECT_LE(heuristic_value, distance);
      diff_sum += distance - heuristic_value;
      ++cnt;
    }
  }
  std::cout << "average underestimation = " << (double)diff_sum / cnt
            << std::endl;
}
