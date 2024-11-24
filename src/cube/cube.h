#pragma once

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

  const CubeLayout &GetCubeLayout() const;

  size_t GetHash() const;

  size_t GetSize() const;

  bool operator==(const Cube &other) const;

  bool operator!=(const Cube &other) const;

private:
  CubeLayout cube_;

  void Swap(const std::vector<std::pair<uint8_t, size_t>> &first,
            const std::vector<std::pair<uint8_t, size_t>> &second);

  std::vector<std::pair<uint8_t, size_t>> GetRow(uint8_t face, uint8_t row,
                                                 bool reversed) const;
  std::vector<std::pair<uint8_t, size_t>> GetCol(uint8_t face, uint8_t col,
                                                 bool reversed) const;
  
  void ClockwiseEdgeRotate(uint8_t face);
  void ClockwiseFaceRotate(uint8_t face);
};

} // namespace cube

namespace std {

template <> struct hash<cube::Cube> {
  size_t operator()(const cube::Cube &cube) const { return cube.GetHash(); }
};

} // namespace std
