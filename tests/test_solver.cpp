#include <gtest/gtest.h>

#include "solver/heuristic_function.h"
#include "solver/solver.h"

#include <random>

std::mt19937 rnd(123);

namespace {

std::vector<cube::Move> getRandomMoves(size_t n) {
  std::uniform_int_distribution<> face_dis(0, 5);
  std::uniform_int_distribution<> rotation_dis(1, 3);
  std::vector<cube::Move> result;
  for (size_t i = 0; i < n; ++i) {
    result.push_back(cube::GetMove(rotation_dis(rnd), face_dis(rnd)));
  }
  return result;
}

void TurnMoves(cube::Cube &cube, const std::vector<cube::Move> &moves) {
  for (auto move : moves) {
    cube.Turn(move);
  }
}

solver::Solver GetSolver() {
  return solver::Solver(solver::HeuristicFunction(), 20);
}

} // namespace

TEST(Solver, TestRandom) {
  auto solver = GetSolver();
  for (size_t i = 0; i < 3; ++i) {
    auto random_moves = getRandomMoves(5);
    std::cout << "random_moves=" << cube::MovesToString(random_moves)
              << std::endl;
    auto cube = cube::Cube();
    TurnMoves(cube, random_moves);
    auto solution = solver.Solve(cube);
    ASSERT_TRUE(solution.has_value());
    std::cout << "solution=" << cube::MovesToString(solution.value())
              << std::endl;
    TurnMoves(cube, solution.value());
    ASSERT_TRUE(cube.IsSolved());
  }
}

TEST(Solver, TestDefault) {
  auto solver = GetSolver();
  std::vector<std::string> moves_strings = {
      "B2L1B2R3F3", "R1U2R1D2R2", "D2R2F2D2F2", "D3F3U1B2R2", "U2R2F2D3U1"};
  for (auto moves_string : moves_strings) {
    std::cout << "moves_string=" << moves_string << std::endl;
    auto moves = cube::GetMovesFromString(moves_string);
    ASSERT_EQ(moves.size(), 5);
    auto cube = cube::Cube();
    TurnMoves(cube, moves);
    auto solution = solver.Solve(cube);
    ASSERT_TRUE(solution.has_value());
    std::cout << "solution=" << cube::MovesToString(solution.value())
              << std::endl;
    ASSERT_EQ(solution.value().size(), 5);
    TurnMoves(cube, solution.value());
    ASSERT_TRUE(cube.IsSolved());
  }
}

TEST(Solver, TestSolutionSize) {
  auto solver = GetSolver();
  std::vector<std::string> moves_strings = {
      "B1D3L3F3L1F1B1U3D2F3R2B3U1F2R3L1U2R2F2B2",
      "U2L3U2F2L3R1D2L2B3D2L1F3R3U3L1U2F3D3R1B1",
      "F3L1B2R1U3B3L1U2D3F1L3R2U2D2B2R2D1R2L2F2",
      "U2R2D2B1U2B3F1D3B3R3D1U2B2F2R3D3B1U3F3R2"};
  size_t cnt = 0;
  for (size_t distance = 1; distance <= 8; ++distance) {
    for (auto moves_string : moves_strings) {
      auto moves =
          cube::GetMovesFromString(moves_string.substr(0, distance * 2));
      // std::cout << moves_string.substr(0, distance * 2) << std::endl;
      auto cube = cube::Cube();
      TurnMoves(cube, moves);
      auto solution = solver.Solve(cube);
      ASSERT_TRUE(solution.has_value());
      ASSERT_EQ(solution.value().size(), distance);
    }
  }
}