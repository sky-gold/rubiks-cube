#pragma once

#include "../cube/cube.h"

namespace heuristic {

class CoordinateCalculator {
public:
    virtual ~CoordinateCalculator() = default;

    virtual size_t GetCoordinate(const cube::Cube& cube) const = 0;

    virtual cube::Cube GetCoordinateCube(const size_t coord) const = 0;

    virtual size_t Size() const = 0;
};

} // namespace heuristic