#include <gtest/gtest.h>

#include "heuristic/heuristic_function.h"
#include "solver/solver.h"

#include <random>

namespace {

std::mt19937 rnd(123);

void TurnMoves(cube::Cube& cube, const std::vector<cube::Move>& moves) {
    for (auto move : moves) {
        cube.Turn(move);
    }
}

std::vector<cube::Move> GetAntiMoves(const std::vector<cube::Move>& moves) {
    std::vector<cube::Move> result;
    for (auto move : moves) {
        result.push_back(cube::GetAntiMove(move));
    }
    std::reverse(result.begin(), result.end());
    return result;
}

std::vector<cube::Move> GetRandomMoves(size_t n) {
    std::uniform_int_distribution<> face_dis(0, 5);
    std::uniform_int_distribution<> rotation_dis(1, 3);
    std::vector<cube::Move> result;
    for (size_t i = 0; i < n; ++i) {
        result.push_back(cube::GetMove(rotation_dis(rnd), face_dis(rnd)));
    }
    return result;
}

solver::Solver GetSolver() { return solver::Solver(std::move(heuristic::HeuristicFunction())); }

}  // namespace

TEST(AntiCube, TestEqual) {
    const auto moves_string = "B2L1B2R3F3U3B3L1D3";
    const auto moves = cube::GetMovesFromString(moves_string);
    const auto anti_moves = GetAntiMoves(moves);
    auto first_cube = cube::Cube();
    auto second_cube = cube::Cube();
    TurnMoves(first_cube, moves);
    TurnMoves(second_cube, anti_moves);
    ASSERT_EQ(first_cube.GetAntiCube(), second_cube);
    ASSERT_EQ(first_cube, second_cube.GetAntiCube());
}

TEST(AntiCube, TestEqualRandom) {
    for (size_t i = 0; i < 100000; ++i) {
        const auto moves = GetRandomMoves(100);
        const auto anti_moves = GetAntiMoves(moves);
        auto first_cube = cube::Cube();
        auto second_cube = cube::Cube();
        TurnMoves(first_cube, moves);
        TurnMoves(second_cube, anti_moves);
        ASSERT_EQ(first_cube.GetAntiCube(), second_cube);
        ASSERT_EQ(first_cube, second_cube.GetAntiCube());
    }
}

TEST(AntiCube, TestEqualDistanceRandom) {
    auto solver = GetSolver();
    for (size_t i = 0; i < 100; ++i) {
        const auto moves = GetRandomMoves(14);
        auto cube = cube::Cube();
        TurnMoves(cube, moves);
        auto anti_cube = cube.GetAntiCube();
        auto cube_solution = solver.Solve(cube);
        auto anti_cube_solution = solver.Solve(anti_cube);
        ASSERT_TRUE(cube_solution.has_value());
        ASSERT_TRUE(anti_cube_solution.has_value());
        ASSERT_EQ(cube_solution.value().size(), anti_cube_solution.value().size());
    }
}
