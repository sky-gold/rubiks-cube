#include "cube.h"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>
#include <stdexcept>

namespace cube {

const std::array<uint8_t, EDGE_INDEX_CNT> start_edge_cubies = {
    {GetEdgeCubie(0, 0), GetEdgeCubie(1, 0), GetEdgeCubie(2, 0),
     GetEdgeCubie(3, 0), GetEdgeCubie(4, 0), GetEdgeCubie(5, 0),
     GetEdgeCubie(6, 0), GetEdgeCubie(7, 0), GetEdgeCubie(8, 0),
     GetEdgeCubie(9, 0), GetEdgeCubie(10, 0), GetEdgeCubie(11, 0)}};

const std::array<uint8_t, CORNER_INDEX_CNT> start_corner_cubies =
    {{GetCornerCubie(0, 0), GetCornerCubie(1, 0), GetCornerCubie(2, 0),
      GetCornerCubie(3, 0), GetCornerCubie(4, 0), GetCornerCubie(5, 0),
      GetCornerCubie(6, 0), GetCornerCubie(7, 0)}};

Cube::Cube(): edge_cubies_(start_edge_cubies), corner_cubies_(start_corner_cubies)  {
}

bool Cube::IsSolved() const {
  return edge_cubies_ == start_edge_cubies && corner_cubies_ == start_corner_cubies;
}

void Cube::Turn(Move move) {
  const std::array<uint8_t, EDGE_CUBIE_CNT> &e_p = edge_permutation[move];
  for (size_t i = 0; i < edge_cubies_.size(); ++i) {
    edge_cubies_[i] = e_p[edge_cubies_[i]];
  }
  const std::array<uint8_t, CORNER_CUBIE_CNT> &c_p = corner_permutation[move];
  for (size_t i = 0; i < corner_cubies_.size(); ++i) {
    corner_cubies_[i] = c_p[corner_cubies_[i]];
  }
}

size_t Cube::GetHash() const {
  uint64_t result;
  std::memcpy(&result, corner_cubies_.data(), sizeof(result));
  return result;
  // size_t seed = 0;

  // for (auto to : edge_cubies_) {
  //   seed ^= to + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  // }
  // for (auto to : corner_cubies_) {
  //   seed ^= to + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  // }
  // return seed;
}

bool Cube::operator==(const Cube &other) const {
  return edge_cubies_ == other.edge_cubies_ &&
         corner_cubies_ == other.corner_cubies_;
}

bool Cube::operator!=(const Cube &other) const {
  return edge_cubies_ != other.edge_cubies_ ||
         corner_cubies_ != other.corner_cubies_;
}

bool Cube::operator<(const Cube &other) const {
  return std::tie(edge_cubies_, corner_cubies_) <
         std::tie(other.edge_cubies_, other.corner_cubies_);
}

const std::array<uint8_t, EDGE_INDEX_CNT> &Cube::GetEdgeCubies() const {
  return edge_cubies_;
}

const std::array<uint8_t, CORNER_INDEX_CNT> &Cube::GetCornerCubies() const {
  return corner_cubies_;
}

} // namespace cube