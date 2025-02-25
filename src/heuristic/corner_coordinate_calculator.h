#pragma once

#include "coordinate_calculator.h"

#include <array>
#include <cstdint>


namespace heuristic {

class CornerCoordinateCalculator : public CoordinateCalculator {
public:
    CornerCoordinateCalculator() = default;

    size_t GetCoordinate(const cube::Cube& cube) const override;
    
    cube::Cube GetCoordinateCube(const size_t coord) const override;

    size_t Size() const override;
};

} // namespace heuristic
