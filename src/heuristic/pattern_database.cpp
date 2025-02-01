#include "pattern_database.h"

#include <iostream>

namespace heuristic {

PatternDatabase::PatternDatabase() {}

size_t PatternDatabase::Get(const cube::Cube &cube) const {
    size_t coordinate = GetCoordinate(cube);  // Используем виртуальный метод
    return distance_.Get(coordinate);         // Возвращаем значение из PackedArray
}

void PatternDatabase::LoadOrCalcAndWrite() {
    try {
        distance_ = utils::PackedArray(Size());
        distance_.ReadFromFile(GetFilename());
    } catch (const std::exception &e) {
        std::cerr << "Failed to load data: " << e.what() << std::endl;
        Calc();
        distance_.WriteToFile(GetFilename());
    }
}

void PatternDatabase::Calc() {
    for (size_t i = 0; i < distance_.Size(); ++i) {
        distance_.Set(i, 0x0F);
    }
    const auto start = cube::Cube();
    std::vector<cube::Cube> q = {start};
    distance_.Set(GetCoordinate(start), 0);

    for (size_t i = 0; i < q.size(); ++i) {
        if (i == 0 || i % (distance_.Size() / 400) == 0) {
            std::cerr << "Calcing " << GetFilename() << " " << i << "/" << distance_.Size() << " (" << ((i * 100) / distance_.Size()) << "%)"
                      << std::endl;
        }
        const auto cur = q[i];
        const auto cur_distance = distance_.Get(GetCoordinate(cur));
        for (int face = 0; face < 6; ++face) {
            for (int rotation = 1; rotation <= 3; ++rotation) {
                auto move = cube::GetMove(rotation, face);
                auto neighbor = cur;
                neighbor.Turn(move);
                if (distance_.Get(GetCoordinate(neighbor)) != 0x0F) {
                    continue;
                }
                q.push_back(neighbor);
                auto neighbor_distance = cur_distance + 1;
                if (neighbor_distance == 0x0F) {
                    throw std::runtime_error("some of distance break max limit");
                }
                distance_.Set(GetCoordinate(neighbor), neighbor_distance);
            }
        }
    }
    std::cerr << "Check " << q.size() << " positions" << std::endl;
}

} // namespace heuristic
