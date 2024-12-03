#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace cube {

using Move = uint8_t;

/*
2 bits = rotation index (1 - clockwise, 2 - 180 degrees turn, 3 - anticlockwise)
3 bits = face
*/

constexpr auto FACE_CNT = 6;
constexpr auto MOVE_CNT = (2 << 3) + FACE_CNT;

namespace Face {

constexpr uint8_t U = 0;
constexpr uint8_t D = 1;
constexpr uint8_t F = 2;
constexpr uint8_t B = 3;
constexpr uint8_t L = 4;
constexpr uint8_t R = 5;

} // namespace Face

inline uint8_t getRotation(Move move) { return (move >> 3) + 1; }

inline uint8_t GetFace(Move move) { return move & 0x07; }

inline Move GetMove(uint8_t rotation, uint8_t face) {
  return ((rotation - 1) << 3) | face;
}

Move GetMoveFromString(const std::string &move_string);

std::vector<Move> GetMovesFromString(const std::string &moves_string);

std::string MoveToString(Move move);

std::string MovesToString(const std::vector<Move> &moves);

} // namespace cube