#pragma once

#include "../cube/cube.h"
#include "../utils/packed_array.h"

#include <fstream>
#include <stdexcept>

namespace heuristic {

class PatternDatabase {
public:
    PatternDatabase();

    size_t Get(const cube::Cube &cube) const;

    virtual size_t GetCoordinate(const cube::Cube &cube) const = 0;

    virtual std::string GetFilename() const = 0;

    virtual size_t Size() const = 0;

protected:
    void LoadOrCalcAndWrite();

    void Calc();

    utils::PackedArray distance_{0};
};

} // namespace heuristic
