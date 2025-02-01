#pragma once

#include "pattern_database.h"

namespace heuristic {

class CornerPatternDatabase : public PatternDatabase {
public:
    CornerPatternDatabase();

    size_t GetCoordinate(const cube::Cube &cube) const override;

    std::string GetFilename() const override;

    size_t Size() const override;
};

} // namespace heuristic
