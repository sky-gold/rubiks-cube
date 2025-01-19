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
  return solver::Solver(std::move(solver::HeuristicFunction()));
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
      "U2R2D2B1U2B3F1D3B3R3D1U2B2F2R3D3B1U3F3R2"
      "B2U2R2D2R2U3B2F3L2U3B1R3B3F1L1U3L3R3B2L1",
      "D2B2L1F3U1F3B1D3F2B2R3F1L2U2L1F2B2L1U2L3",
      "B1U1D1F3B3R1U1R2B1R3L1B1R3U2B2D1R2D3L2F2",
      "L2F3L2R2U2R2B2U3L1D2U1R3D1L2B1L3F1U1B3R1",
      "U2L2U1L2B2F2D3R1F1D3U1B1R3F3U1R2D1U3L1D1",
      "U3B3U2D2R3D1B3L1U1B2D2R1U2R2F2L1U2R2F2L3",
      "F1R2B1D2U2B3F1U1F3L3D3R1B1L3D2F3L3B3U3B1",
      "L2U3B2R2B2D3R2L1B1F1D1R3B1L3B3D1U3L3D3F1"};
  size_t cnt = 0;
  for (size_t distance = 1; distance <= 9; ++distance) {
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