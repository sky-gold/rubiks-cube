#include <gtest/gtest.h>

#include "solver/solver.h"

#include <random>

namespace {

void TurnMoves(cube::Cube& cube, const std::vector<cube::Move>& moves) {
    for (auto move: moves) {
        cube.Turn(move);
    }
}

}

TEST(Turn, TestIsSolved) {
  auto cube = cube::Cube();
  const auto moves_string = "B2L'B'B'F2F2B2LB2B2FRRF2F2R2F'B2L'B2B2R'U2U2LR";
  const auto moves = cube::GetMovesFromString(moves_string);
  TurnMoves(cube, moves);
  ASSERT_TRUE(cube.IsSolved());
}
