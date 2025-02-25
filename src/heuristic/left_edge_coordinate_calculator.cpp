#include "left_edge_coordinate_calculator.h"

#include "../utils/fast_math.h"

namespace heuristic {

size_t LeftEdgeCoordinateCalculator::GetCoordinate(const cube::Cube& cube) const {
    const std::array<uint8_t, cube::EDGE_INDEX_CNT>& edge_cubies = cube.GetEdgeCubies();
    size_t position_coord = 0;
    size_t rotation_coord = 0;
    uint16_t mask = (1LL << cube::EDGE_INDEX_CNT) - 1;
    for (int i = 0; i < edges_cnt_; ++i) {
        const auto& cubie = edge_cubies[i];
        rotation_coord *= 2;
        rotation_coord += cube::GetEdgeCubieFlip(cubie);
        auto pos = cube::GetEdgeCubiePosition(cubie);
        size_t cnt = __builtin_popcount(mask & ((1 << pos) - 1));
        mask ^= (1 << pos);
        position_coord += (utils::FACTORIAL[cube::EDGE_INDEX_CNT - 1 - i] /
                           utils::FACTORIAL[cube::EDGE_INDEX_CNT - edges_cnt_]) *
                          cnt;
    }
    return utils::POWERS_OF_TWO[edges_cnt_] * position_coord + rotation_coord;
}

cube::Cube LeftEdgeCoordinateCalculator::GetCoordinateCube(const size_t coord) const {
    std::array<uint8_t, cube::EDGE_INDEX_CNT> edge_cubies{};
    std::vector<uint8_t> elems;
    elems.reserve(cube::EDGE_INDEX_CNT);
    for (uint8_t i = 0; i < cube::EDGE_INDEX_CNT; ++i) {
        elems.push_back(i);
    }
    size_t position_coord = coord / utils::POWERS_OF_TWO[edges_cnt_];
    for (size_t i = 0; i < edges_cnt_; ++i) {
        const auto step = utils::FACTORIAL[cube::EDGE_INDEX_CNT - i - 1] / utils::FACTORIAL[cube::EDGE_INDEX_CNT - edges_cnt_];
        size_t elem_index = position_coord / step;
        edge_cubies[i] = cube::GetEdgeCubie(elems[elem_index], 0);
        elems.erase(elems.begin() + elem_index);
        position_coord %= step;
    }
    size_t rotation_coord = coord % utils::POWERS_OF_TWO[edges_cnt_];
    for (int i = edges_cnt_ - 1; i >= 0; --i) {
      edge_cubies[i] |= (rotation_coord % 2);
        rotation_coord /= 2;
    }
    return cube::Cube(edge_cubies, std::array<uint8_t, cube::CORNER_INDEX_CNT>{});
}

size_t LeftEdgeCoordinateCalculator::Size() const {
    return (utils::FACTORIAL[cube::EDGE_INDEX_CNT] /
            utils::FACTORIAL[cube::EDGE_INDEX_CNT - edges_cnt_]) *
           utils::POWERS_OF_TWO[edges_cnt_];
}

}  // namespace heuristic
