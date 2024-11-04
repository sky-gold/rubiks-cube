#pragma once

#include <cstddef>
#include <cstdint>

typedef uint8_t Move;
/*
3 bits = index (the more, the closer to the center)
2 bits = rotation index (1 - clockwise, 2 - 180 degrees turn, 3 - anticlockwise)
3 bits = U, D, L, R, F, B
*/

namespace MoveUtils {

inline uint8_t GetMoveIndex(Move move) { return move >> 5; }

inline uint8_t getRotation(Move move) { return (move >> 3) & 0x03; }

inline uint8_t GetFace(Move move) { return move & 0x07; }

inline Move GetMove(uint8_t move_index, uint8_t rotation, uint8_t face) {
  return (move_index << 5) | (rotation << 3) | face;
}

} // namespace MoveUtils