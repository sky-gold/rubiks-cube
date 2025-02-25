#include "corner_coordinate_calculator.h"

#include "../utils/fast_math.h"

namespace heuristic {

size_t CornerCoordinateCalculator::GetCoordinate(const cube::Cube& cube) const {
    const std::array<uint8_t, cube::CORNER_INDEX_CNT>& corner_cubies = cube.GetCornerCubies();
    size_t rotation_coord = 0;
    for (size_t i = 0; i < cube::CORNER_INDEX_CNT - 1; ++i) {
        rotation_coord *= 3;
        rotation_coord += cube::GetCornerCubieRotation(corner_cubies[i]);
    }
    size_t position_coord = 0;
    uint8_t mask = 0;
    for (int i = cube::CORNER_INDEX_CNT - 1; i >= 0; --i) {
        auto pos = cube::GetCornerCubiePosition(corner_cubies[i]);
        size_t cnt = __builtin_popcount(mask & ((1 << pos) - 1));
        mask |= (1 << pos);
        position_coord += utils::FACTORIAL[cube::CORNER_INDEX_CNT - 1 - i] * cnt;
    }
    return utils::FACTORIAL[cube::CORNER_INDEX_CNT] * rotation_coord + position_coord;
}

cube::Cube CornerCoordinateCalculator::GetCoordinateCube(const size_t coord) const {
  std::array<uint8_t, cube::CORNER_INDEX_CNT> corner_cubies{};
  std::vector<uint8_t> elems;
  elems.reserve(8);
  for (uint8_t i = 0; i < 8; ++i) {
    elems.push_back(i);
  }
  size_t position_coord = coord % utils::FACTORIAL[cube::CORNER_INDEX_CNT];
  for (size_t i = 0; i < cube::CORNER_INDEX_CNT; ++i) {
    size_t elem_index = position_coord / utils::FACTORIAL[cube::CORNER_INDEX_CNT - i - 1];
    corner_cubies[i] = cube::GetCornerCubie(elems[elem_index], 0);
    elems.erase(elems.begin() + elem_index);
    position_coord %= utils::FACTORIAL[cube::CORNER_INDEX_CNT - i - 1];
  }
  size_t rotation_coord = coord / utils::FACTORIAL[cube::CORNER_INDEX_CNT];
  for (int i = cube::CORNER_INDEX_CNT - 2; i >= 0; --i) {
    corner_cubies[i] |= (rotation_coord % 3);
    rotation_coord /= 3;
  }
  return cube::Cube(std::array<uint8_t, cube::EDGE_INDEX_CNT>{}, corner_cubies);
}

size_t CornerCoordinateCalculator::Size() const { return utils::FACTORIAL[8] * utils::POWERS_OF_THREE[7]; }

}  // namespace heuristic
