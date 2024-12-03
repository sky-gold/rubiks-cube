#pragma once

#include "cubie.h"
#include "move.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

constexpr size_t CUBE_SIZE = 3;
constexpr size_t FACES_COUNT = 6;

namespace cube {

using CubeLayout = std::array<std::vector<uint8_t>, FACES_COUNT>;

class Cube {
public:
  Cube();

  bool IsSolved() const;

  void Turn(Move move);

  size_t GetHash() const;

  bool operator==(const Cube &other) const;

  bool operator!=(const Cube &other) const;

private:
  std::array<uint8_t, EDGE_INDEX_CNT> edge_cubies_;
  std::array<uint8_t, CORNER_INDEX_CNT> corner_cubies_;
};

} // namespace cube

namespace std {

template <> struct hash<cube::Cube> {
  size_t operator()(const cube::Cube &cube) const { return cube.GetHash(); }
};

} // namespace std
