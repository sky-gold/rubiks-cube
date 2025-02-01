#include "left_edge_pattern_database.h"

#include "../utils/fast_math.h"

namespace heuristic {

LeftEdgePatternDatabase::LeftEdgePatternDatabase() { LoadOrCalcAndWrite(); }

size_t LeftEdgePatternDatabase::GetCoordinate(const cube::Cube &cube) const {
  const std::array<uint8_t, cube::EDGE_INDEX_CNT> &edge_cubies =
      cube.GetEdgeCubies();
  size_t position_coord = 0;
  size_t rotation_coord = 0;
  uint16_t mask = (1LL << cube::EDGE_INDEX_CNT) - 1;
  for (int i = 0; i < LEFT_EDGE_PATTERN_DATABASE_SIZE; ++i) {
    const auto &cubie = edge_cubies[i];
    rotation_coord *= 2;
    rotation_coord += cube::GetEdgeCubieFlip(cubie);
    auto pos = cube::GetEdgeCubiePosition(cubie);
    size_t cnt = __builtin_popcount(mask & ((1 << pos) - 1));
    mask ^= (1 << pos);
    position_coord += (utils::FACTORIAL[cube::EDGE_INDEX_CNT - 1 - i] /
                       utils::FACTORIAL[cube::EDGE_INDEX_CNT -
                                        LEFT_EDGE_PATTERN_DATABASE_SIZE]) *
                      cnt;
  }
  return utils::POWERS_OF_TWO[6] * position_coord + rotation_coord;
}

std::string LeftEdgePatternDatabase::GetFilename() const {
  return "left_" + std::to_string(LEFT_EDGE_PATTERN_DATABASE_SIZE) +
         "_edge_pattern_database.bin";
}

size_t LeftEdgePatternDatabase::Size() const {
  return (utils::FACTORIAL[cube::EDGE_INDEX_CNT] /
          utils::FACTORIAL[cube::EDGE_INDEX_CNT -
                           LEFT_EDGE_PATTERN_DATABASE_SIZE]) *
         utils::POWERS_OF_TWO[LEFT_EDGE_PATTERN_DATABASE_SIZE];
}

} // namespace heuristic
