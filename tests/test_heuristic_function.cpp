#include <gtest/gtest.h>

#include "heuristic/heuristic_function.h"
#include "solver/solver.h"

#include <random>

namespace {

const std::vector<std::string> moves_strings = {
    "B2L1B2R3F3U3B3L1D3F3L1U1L2B2L3D2B2D2R2B2",
    "R1U2R1D2R2B2L3D3B3F1U1B3R3U2L3D1R2F3U2L2",
    "D2R2F2D2F2D2R3F2D3L2R1B1L3F1U1R3B1F2R2F3",
    "D3F3U1B2R2F1R3U2B3L1D1F1R1D2R2L2D3R2F2D3",
    "U2R2F2D3U1F2U2B1U1B3R1U3F1L1B1R3F1L2D3B1",
    "B1D3L3F3L1F1B1U3D2F3R2B3U1F2R3L1U2R2F2B2",
    "U2L3U2F2L3R1D2L2B3D2L1F3R3U3L1U2F3D3R1B1",
    "F3L1B2R1U3B3L1U2D3F1L3R2U2D2B2R2D1R2L2F2",
    "U2R2D2B1U2B3F1D3B3R3D1U2B2F2R3D3B1U3F3R2",
    "U1F1D3R1L1F3D3R1U2F1U3R1D3F2B2D2B2D1B2R2",
    "R1F1L2F3L2U3B2R3U1B3D1L1F3R3B2L2U2F2U2R2",
    "U2R3F2L3D2L3F2L3D1F3U3B1D2L1D3F3L1F2L3D3",
    "L2B2D2F2L1B2D2F1D3U3B3F2L1R1D1F3D3L2U1R2",
    "L2B3F1D2B1L2B1R3U2L3D3R1F3D3U1F3L3D3L1R1",
    "R2D2B2F2R2B2F2R1U3F3R2D3B2L3U2F1R3U1R2B3",
    "R2B2F2D2L1R2F2R1U3R2B3D1F1U2B2U1F1R3F3L2",
    "U2F1L2F2U2D2F3U2F1L1F1U3D3L1B3U2R1U2B1U3",
    "B1F1D2L2F1R2F3R2U1F3R3D1L3B3U3F3R1F3D3L2",
    "R3F1D3L2D2L3F3U1F2D2L1R2D2F1R2B1L2B2U2D2",
    "D1R3F1D1B1U3B2R2U3B1R1B3D2F2B2R1D2B2R1L3",
    "B2L3F2R1F2R3B2D2U2B3R3U2F1R1D3B1L1U1F3D2",
    "R1F3L1B2D1B3D3F1U1F3D1L1U2L1B2U2B2L3D2R3",
    "U3R2F2U1F2D2U1B1D1R3B1L3D3L3B2F3D2L1B1R2",
    "D2L2F2R2U3L2U2B3D2R1F3D3B1R3F2L3F2R3D3R3",
    "B2U2R2D2R2U3B2F3L2U3B1R3B3F1L1U3L3R3B2L1",
    "D2B2L1F3U1F3B1D3F2B2R3F1L2U2L1F2B2L1U2L3",
    "B1U1D1F3B3R1U1R2B1R3L1B1R3U2B2D1R2D3L2F2",
    "L2F3L2R2U2R2B2U3L1D2U1R3D1L2B1L3F1U1B3R1",
    "U2L2U1L2B2F2D3R1F1D3U1B1R3F3U1R2D1U3L1D1",
    "U3B3U2D2R3D1B3L1U1B2D2R1U2R2F2L1U2R2F2L3",
    "F1R2B1D2U2B3F1U1F3L3D3R1B1L3D2F3L3B3U3B1",
    "L2U3B2R2B2D3R2L1B1F1D1R3B1L3B3D1U3L3D3F1",
    "L1D2L2R1B2U2R3U1R2U2F2D2R3B1F1U3B1L1B1R1",
    "L2D1R2D3F2U2B2U1R3U3B3R1F3R3B2D1L3F3R3F3",
    "B2D2R2B2U3B2F2L1F3U3B3R1B3D2L3R2B1L3B2F1",
    "F3U2B1U2B1R2U2B2U3R1F1L2F1U1F3L3D3B3U2R1",
    "U3L1B2L3D3F3U2R3U3F1B3D2L2U2D2L3U2R1L2F2",
    "U3B3R1L3F1U1R1B2L1U3D2B2U3R3D2F2R3L2U2L2",
    "D2L1U2L1F2R1D2F2U2F1U3B3U2B2U3R3B2D1B1L1",
    "R2D1R2U3B2D1U1L3F3U3B3D3L1D2F3L1D2F2R3D2",
    "R3D1U1L1F3B3R3D3B3L3B1D2L2D2L1B2L1D2B2L3",
    "R2F2D2F2U2D2R1U3F2D2B1U1D3L3F3L3B3U3R3F2",
    "L2U1F2D3F2U2R2U3F3R3U1F1U1L1U2B3D1U3F1R3",
    "L1B1R1L3F2D1R1L1U3D3F1D1L1B1R2U2D2F2U2B2",
    "F2U1F3U2F3D1F3R3B3L3U1F1L1B2L2F2R2U2R3D2",
    "U1F2L2D1F1D2B3U1D2R3B1L1D1L1U3R2L2U1R2U3",
    "U2L3U2L1B2R2U2R3U1R3B1L2U1B2D1L3B1D3F3R3",
    "B1U2L2F3R2B1F1U2L1D3F3R1F3D3F3R2U3L2F1L1"};

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

TEST(HeuristicFunction, IsLowerThanActualDistanceToStartPosition) {
  heuristic::HeuristicFunction heuristic_function;
  size_t diff_sum = 0;
  size_t cnt = 0;
  size_t max_heuristic_value = 0;
  for (size_t distance = 1; distance <= 20; ++distance) {
    for (auto moves_string : moves_strings) {
      const auto cube =
          CubeFromMovesString(moves_string.substr(0, distance * 2));
      const auto heuristic_value = heuristic_function.GetHeuristic(cube);
      EXPECT_LE(heuristic_value, distance);
      diff_sum += distance - heuristic_value;
      ++cnt;
      if (max_heuristic_value < heuristic_value) {
        max_heuristic_value = heuristic_value;
      }
    }
  }
  std::cout << "average underestimation = " << (double)diff_sum / cnt
            << std::endl;
  std::cout << "max_heuristic_value = " << max_heuristic_value
            << std::endl;
  
}

