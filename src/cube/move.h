#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace cube {

using Move = uint8_t;

/*
3 bits = index (the more, the closer to the center)
2 bits = rotation index (1 - clockwise, 2 - 180 degrees turn, 3 - anticlockwise)
3 bits = face
*/

namespace Face {

constexpr uint8_t U = 0;
constexpr uint8_t D = 1;
constexpr uint8_t F = 2;
constexpr uint8_t B = 3;
constexpr uint8_t L = 4;
constexpr uint8_t R = 5;

} // namespace Face

inline uint8_t GetMoveIndex(Move move) { return move >> 5; }

inline uint8_t getRotation(Move move) { return (move >> 3) & 0x03; }

inline uint8_t GetFace(Move move) { return move & 0x07; }

inline Move GetMove(uint8_t move_index, uint8_t rotation, uint8_t face) {
  return (move_index << 5) | (rotation << 3) | face;
}

Move GetMoveFromString(const std::string &move_string);

std::vector<Move> GetMovesFromString(const std::string &moves_string);

} // namespace cube