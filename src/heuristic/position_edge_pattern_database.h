#pragma once

#include "pattern_database.h"

namespace heuristic {

class PositionEdgePatternDatabase : public PatternDatabase {
public:
    PositionEdgePatternDatabase();

    size_t GetCoordinate(const cube::Cube &cube) const override;

    std::string GetFilename() const override;

    size_t Size() const override;
};

} // namespace heuristic
