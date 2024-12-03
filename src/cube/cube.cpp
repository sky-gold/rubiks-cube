#include "cube.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <stdexcept>

namespace cube {

Cube::Cube() {
  for (size_t i = 0; i < edge_cubies_.size(); ++i) {
    edge_cubies_[i] = GetEdgeCubie(i, 0);
  }
  for (size_t i = 0; i < corner_cubies_.size(); ++i) {
    corner_cubies_[i] = GetCornerCubie(i, 0);
  }
}

bool Cube::IsSolved() const {
  for (size_t i = 0; i < edge_cubies_.size(); ++i) {
    if (edge_cubies_[i] != GetEdgeCubie(i, 0)) {
      return false;
    }
  }
  for (size_t i = 0; i < corner_cubies_.size(); ++i) {
    if (corner_cubies_[i] != GetCornerCubie(i, 0)) {
      return false;
    }
  }
  return true;
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
  size_t seed = 0;

  for (auto to : edge_cubies_) {
    seed ^= to + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  }
  for (auto to : corner_cubies_) {
    seed ^= to + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  }
  return seed;
}

bool Cube::operator==(const Cube &other) const {
  return edge_cubies_ == other.edge_cubies_ &&
         corner_cubies_ == other.corner_cubies_;
}

bool Cube::operator!=(const Cube &other) const {
  return edge_cubies_ != other.edge_cubies_ ||
         corner_cubies_ != other.corner_cubies_;
}

} // namespace cube