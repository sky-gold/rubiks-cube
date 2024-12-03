#pragma once

#include "move.h"

namespace cube {

constexpr auto CORNER_CUBIE_CNT = (7 << 2) + 3;
constexpr auto EDGE_CUBIE_CNT = (11 << 1) + 2;

constexpr auto CORNER_INDEX_CNT = 8;
constexpr auto EDGE_INDEX_CNT = 12;

extern const std::array<std::array<uint8_t, CORNER_CUBIE_CNT>, MOVE_CNT>
    corner_permutation;
extern const std::array<std::array<uint8_t, EDGE_CUBIE_CNT>, MOVE_CNT>
    edge_permutation;

inline uint8_t GetEdgeCubieFlip(uint8_t edge_cubie) { return edge_cubie & 1; }

inline uint8_t GetCornerCubieRotation(uint8_t corner_cubie) {
  return corner_cubie & 3;
}

inline uint8_t GetEdgeCubiePosition(uint8_t edge_cubie) {
  return edge_cubie >> 1;
}

inline uint8_t GetCornerCubiePosition(uint8_t corner_cubie) {
  return corner_cubie >> 2;
}

inline uint8_t GetEdgeCubie(uint8_t pos, uint8_t flip) {
  return (pos << 1) + flip;
}

inline uint8_t GetCornerCubie(uint8_t pos, uint8_t rotation) {
  return (pos << 2) + rotation;
}

void CalcPermutationTable();

} // namespace cube