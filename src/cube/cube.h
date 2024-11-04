#pragma once

#include <cstddef>
#include <cstdint>

#include "move_utils.h"

constexpr size_t MAX_CUBE_SIZE = 17;
constexpr size_t MIN_CUBE_SIZE = 2;
constexpr size_t FACES_COUNT = 6;


enum class Face : uint8_t { U = 0, D = 1, F = 2, B = 3, L = 4, R = 5 };

template <size_t N> class Cube {
public:
  Cube();

  virtual bool IsSolved() const = 0;

  virtual void Turn(Move move) = 0;
};

template <size_t N> Cube<N>::Cube() {
  static_assert(MIN_CUBE_SIZE <= N && N <= MAX_CUBE_SIZE,
                "Cube size must be between 2x2x2 and 17x17x17");
}