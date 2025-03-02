#include <gtest/gtest.h>

#include "heuristic/position_edge_coordinate_calculator.h"

#include <random>

namespace {

std::mt19937 rnd(123);

std::unique_ptr<heuristic::CoordinateCalculator> GetCalculator(const size_t edges_cnt) {
    return std::make_unique<heuristic::PositionEdgeCoordinateCalculator>(edges_cnt);
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
    for (auto move : moves) {
        cube.Turn(move);
    }
    return cube;
}

}  // namespace

TEST(PositionEdgeCoordinateCalculator, GetCoordinate) {
    auto calculator = GetCalculator(6);
    auto cube = cube::Cube(
        {
            cube::GetEdgeCubie(0, 0),
            cube::GetEdgeCubie(1, 1),
            cube::GetEdgeCubie(2, 0),
            cube::GetEdgeCubie(3, 1),
            cube::GetEdgeCubie(4, 0),
            cube::GetEdgeCubie(5, 1),
            cube::GetEdgeCubie(11, 0),
            cube::GetEdgeCubie(10, 0),
            cube::GetEdgeCubie(9, 0),
            cube::GetEdgeCubie(8, 0),
            cube::GetEdgeCubie(7, 0),
            cube::GetEdgeCubie(6, 0),
        },
        {}
    );
    auto coord = calculator->GetCoordinate(cube);
    auto coord_cube = calculator->GetCoordinateCube(coord);
    EXPECT_EQ(cube, coord_cube);
}

TEST(PositionEdgeCoordinateCalculator, GetCoordinateRandomCube) {
    for (size_t edges_cnt = 4; edges_cnt <= 6; ++edges_cnt) {
        auto calculator = GetCalculator(edges_cnt);
        for (size_t i = 0; i < 10000; ++i) {
            auto random_cube = GetRandomCube();
            auto random_cube_coord = calculator->GetCoordinate(random_cube);
            auto coord_cube = calculator->GetCoordinateCube(random_cube_coord);
            EXPECT_EQ(random_cube_coord, calculator->GetCoordinate(coord_cube));
        }
    }
}

TEST(PositionEdgeCoordinateCalculator, RandomCubeCoordLowerThanSize) {
    for (size_t edges_cnt = 4; edges_cnt <= 6; ++edges_cnt) {
        auto calculator = GetCalculator(edges_cnt);
        for (size_t i = 0; i < 10000; ++i) {
            auto random_cube = GetRandomCube();
            auto random_cube_coord = calculator->GetCoordinate(random_cube);
            EXPECT_LT(random_cube_coord, calculator->Size());
        }
    }
}
