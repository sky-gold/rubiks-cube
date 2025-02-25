#include <gtest/gtest.h>

#include "heuristic/corner_coordinate_calculator.h"

#include <random>

namespace {

std::mt19937 rnd(123);

std::unique_ptr<heuristic::CoordinateCalculator> GetCalculator() {
    return std::make_unique<heuristic::CornerCoordinateCalculator>();
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

cube::Cube GetRandomCube() {
    auto moves = GetRandomMoves(100);
    auto cube = cube::Cube();
    for (auto move: moves) {
        cube.Turn(move);
    }
    return cube;
}

}  // namespace

TEST(CornerCoordinateCalculator, GetCoordinate) {
    auto calculator = GetCalculator();
    auto cube = cube::Cube(
        {},
        {cube::GetCornerCubie(0, 1),
         cube::GetCornerCubie(1, 0),
         cube::GetCornerCubie(2, 2),
         cube::GetCornerCubie(3, 1),
         cube::GetCornerCubie(4, 0),
         cube::GetCornerCubie(5, 2),
         cube::GetCornerCubie(6, 1),
         cube::GetCornerCubie(7, 0)}
    );
    auto coord = calculator->GetCoordinate(cube);
    auto coord_cube = calculator->GetCoordinateCube(coord);
    EXPECT_EQ(cube, coord_cube);
}

TEST(CornerCoordinateCalculator, GetCoordinateRandomCube) {
    auto calculator = GetCalculator();
    for (size_t i = 0; i < 10000; ++i) {
        auto random_cube = GetRandomCube();
        auto random_cube_coord = calculator->GetCoordinate(random_cube);
        auto coord_cube = calculator->GetCoordinateCube(random_cube_coord);
        EXPECT_EQ(random_cube_coord, calculator->GetCoordinate(coord_cube));
    }
}

TEST(CornerCoordinateCalculator, RandomCubeCoordLowerThanSize) {
    auto calculator = GetCalculator();
    for (size_t i = 0; i < 10000; ++i) {
        auto random_cube = GetRandomCube();
        auto random_cube_coord = calculator->GetCoordinate(random_cube);
        EXPECT_LT(random_cube_coord, calculator->Size());
    }
}
