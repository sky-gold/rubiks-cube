#pragma once

#include "../cube/move.h"
#include "../cube/cube.h"

#include <optional>


namespace solver {

class Solver {
public:
    Solver(size_t threshold = 20);

    std::optional<std::vector<cube::Move>> Solve(const cube::Cube& start);
private:
    const size_t threshold_;
};


}