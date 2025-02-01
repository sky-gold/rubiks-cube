#pragma once

#include "pattern_database.h"

namespace heuristic {

const size_t LEFT_EDGE_PATTERN_DATABASE_SIZE = 7;

class LeftEdgePatternDatabase : public PatternDatabase {
public:
    LeftEdgePatternDatabase();

    size_t GetCoordinate(const cube::Cube &cube) const override;

    std::string GetFilename() const override;

    size_t Size() const override;
};

} // namespace heuristic
