#include "heuristic_function.h"
#include <iostream>
#include <stdexcept>

#include "corner_coordinate_calculator.h"
#include "left_edge_coordinate_calculator.h"
#include "right_edge_coordinate_calculator.h"
#include "position_edge_coordinate_calculator.h"


namespace heuristic {

HeuristicFunction::HeuristicFunction(PDBSize size) {
    switch(size) {
        case PDBSize::Low: // 44.1 MB
            pdbs_.emplace_back(PatternDatabaseHeuristic(0, 4, std::make_unique<CornerCoordinateCalculator>(), "cpdb.bin", 11));
            break;
            
        case PDBSize::Mid: // 5.2 GB
            pdbs_.emplace_back(PatternDatabaseHeuristic(0, 4, std::make_unique<LeftEdgeCoordinateCalculator>(8), "l8epdb.bin"));
            pdbs_.emplace_back(
                PatternDatabaseHeuristic(0, 4, std::make_unique<RightEdgeCoordinateCalculator>(8), "r8epdb.bin", 12)
            );
            pdbs_.emplace_back(PatternDatabaseHeuristic(0, 4, std::make_unique<CornerCoordinateCalculator>(), "cpdb.bin", 11));
            break;
            
        case PDBSize::High: // 13 GB
            pdbs_.emplace_back(PatternDatabaseHeuristic(0, 4, std::make_unique<LeftEdgeCoordinateCalculator>(8), "l8epdb.bin"));
            pdbs_.emplace_back(
                PatternDatabaseHeuristic(0, 4, std::make_unique<RightEdgeCoordinateCalculator>(8), "r8epdb.bin", 12)
            );
            pdbs_.emplace_back(PatternDatabaseHeuristic(0, 4, std::make_unique<CornerCoordinateCalculator>(), "cpdb.bin", 11));
            break;
    }
    
    
}

size_t HeuristicFunction::GetHeuristic(const cube::Cube& cube) const {
    if (pdbs_.empty()) {
        return 0;
    }
    size_t result = 0;
    const auto anti_cube = cube.GetAntiCube();
    for (size_t i = 0; i < pdbs_.size(); ++i) {
        if (result >= pdbs_[i].GetMaxValue()) {
            continue;
        }
        size_t new_result = pdbs_[i].GetHeuristic(cube);
        if (new_result > result) {
            result = new_result;
        }
        new_result = pdbs_[i].GetHeuristic(anti_cube);
        if (new_result > result) {
          result = new_result;
        }
    }
    return result;
}

}  // namespace heuristic
