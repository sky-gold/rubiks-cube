#pragma once

#include "coordinate_calculator.h"

#include <array>
#include <cstdint>


namespace heuristic {

class PositionEdgeCoordinateCalculator : public CoordinateCalculator {
public:
    PositionEdgeCoordinateCalculator(size_t edges_cnt = 6): edges_cnt_(edges_cnt) {};

    size_t GetCoordinate(const cube::Cube& cube) const override;
    
    cube::Cube GetCoordinateCube(const size_t coord) const override;

    size_t Size() const override;
private:
    const size_t edges_cnt_;
};

} // namespace heuristic
