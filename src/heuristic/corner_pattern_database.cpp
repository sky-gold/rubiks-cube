#include "corner_pattern_database.h"

#include "../utils/fast_math.h"

namespace heuristic {

CornerPatternDatabase::CornerPatternDatabase() { LoadOrCalcAndWrite(); }

size_t CornerPatternDatabase::GetCoordinate(const cube::Cube &cube) const {
  const std::array<uint8_t, cube::CORNER_INDEX_CNT> &corner_cubies =
      cube.GetCornerCubies();
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
  return utils::FACTORIAL[cube::CORNER_INDEX_CNT] * rotation_coord +
         position_coord;
}

std::string CornerPatternDatabase::GetFilename() const {
  return "corner_pattern_database.bin";
}

size_t CornerPatternDatabase::Size() const {
  return utils::FACTORIAL[8] * utils::POWERS_OF_THREE[7];
}

} // namespace heuristic
