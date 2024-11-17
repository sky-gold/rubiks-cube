#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

#include "move.h"

constexpr size_t MAX_CUBE_SIZE = 17;
constexpr size_t MIN_CUBE_SIZE = 2;
constexpr size_t FACES_COUNT = 6;

namespace cube {

using CubeLayout = std::array<std::vector<uint8_t>, FACES_COUNT>;

class Cube {
public:
  Cube(size_t n);

  virtual bool IsSolved() const = 0;

  virtual void Turn(Move move) = 0;

  virtual const CubeLayout &GetCubeLayout() const = 0;

  virtual size_t GetHash() const = 0;

  size_t GetSize() const;

protected:
  const size_t n_;
};

} // namespace cube