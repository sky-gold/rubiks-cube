#pragma once

#include "coordinate_calculator.h"
#include "../utils/packed_array.h"

#include <memory>

namespace heuristic {

class PatternDatabaseHeuristic {
public:
    PatternDatabaseHeuristic(size_t base, unsigned int bits_per_element, std::unique_ptr<CoordinateCalculator>&& calculator, const std::string& filename, size_t max_value=20);

    ~PatternDatabaseHeuristic() = default;

    PatternDatabaseHeuristic(const PatternDatabaseHeuristic&) = delete;
    PatternDatabaseHeuristic& operator=(const PatternDatabaseHeuristic&) = delete;

    PatternDatabaseHeuristic(PatternDatabaseHeuristic&&) noexcept = default;
    PatternDatabaseHeuristic& operator=(PatternDatabaseHeuristic&&) noexcept = default;

    size_t GetHeuristic(const cube::Cube& cube) const;
    size_t GetMaxValue() const;

private:
    size_t base_;
    unsigned int bits_per_element_;
    std::unique_ptr<CoordinateCalculator> calculator_;
    std::string filename_;
    std::unique_ptr<utils::PackedArray> database_;
    size_t max_value_;

    std::vector<size_t> GetNeighbors(size_t coord);

    void CalcBFS();

    void CalcBFSInplaceQueueless();

    // struct DataDLS{
    //     utils::PackedArray distance;
    //     bool found_new = false;
    //     size_t total_processed = 0;
    //     size_t max_depth = 0;
    //     std::vector<cube::Move> path{};
    //     cube::Cube cube;
    //     bool logged = false;
    // };

    // void CalcUsingIDDFS();
    // void DLS(DataDLS & data, size_t depth);
};

} // namespace heuristic