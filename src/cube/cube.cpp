#include "cube.h"
#include <stdexcept>

namespace cube {
Cube::Cube(size_t n) : n_(n) {
  if (n < MIN_CUBE_SIZE || n > MAX_CUBE_SIZE) {
    throw std::logic_error("Cube size must be between 2 and 17");
  }
}

size_t Cube::GetSize() const {
  return n_;
}

} // namespace cube