#include "position_edge_pattern_database.h"

#include "../utils/fast_math.h"

namespace heuristic {

PositionEdgePatternDatabase::PositionEdgePatternDatabase() { LoadOrCalcAndWrite(); }

size_t PositionEdgePatternDatabase::GetCoordinate(const cube::Cube &cube) const {
    const std::array<uint8_t, cube::EDGE_INDEX_CNT> &edge_cubies =
      cube.GetEdgeCubies();
  size_t position_coord = 0;
  uint16_t mask = 0;
  for (int i = cube::EDGE_INDEX_CNT - 1; i >= 0; --i) {
    auto pos = cube::GetEdgeCubiePosition(edge_cubies[i]);
    size_t cnt = __builtin_popcount(mask & ((1 << pos) - 1));
    mask |= (1 << pos);
    position_coord += utils::FACTORIAL[cube::EDGE_INDEX_CNT - 1 - i] * cnt;
  }
  return position_coord;
}

std::string PositionEdgePatternDatabase::GetFilename() const {
  return "position_edge_pattern_database.bin";
}

size_t PositionEdgePatternDatabase::Size() const {
  return utils::FACTORIAL[12];
}

} // namespace heuristic
