#include "corner_pattern_database.h"
#include <iostream>

namespace solver {

CornerPatternDatabase::CornerPatternDatabase() { 
  LoadOrCalcAndWrite(); 
}

void CornerPatternDatabase::LoadOrCalcAndWrite() {
  try {
    distance_.ReadFromFile(filename_);
  } catch (const std::exception &e) {
    std::cerr << "Failed to load data: " << e.what() << std::endl;
    Calc();
    distance_.WriteToFile(filename_);
  }
}

size_t CornerPatternDatabase::GetCornerCoordinate(const cube::Cube &cube) const{
  const std::array<uint8_t, cube::CORNER_INDEX_CNT> &corner_cubies =
      cube.GetCornerCubies();
  size_t rotation_coord = 0;
  for (size_t i = 0; i < cube::CORNER_INDEX_CNT - 1; ++i) {
    rotation_coord *= 3;
    rotation_coord += cube::GetCornerCubieRotation(corner_cubies[i]);
  }
  std::array<size_t, cube::CORNER_INDEX_CNT + 1> factorial_arr;
  for (size_t i = 0; i < cube::CORNER_INDEX_CNT + 1; ++i) {
    if (i == 0) {
        factorial_arr[i] = 1;
    } else {
        factorial_arr[i] = factorial_arr[i - 1] * i;
    }
  }
  size_t position_coord = 0;
  for (size_t i = 0; i < cube::CORNER_INDEX_CNT; ++i) {
    size_t cnt = 0;
    for (size_t j = i + 1; j < cube::CORNER_INDEX_CNT; ++j) {
        if (cube::GetCornerCubiePosition(corner_cubies[i]) > cube::GetCornerCubiePosition(corner_cubies[j])) {
            ++cnt;
        }
    }
    position_coord += factorial_arr[cube::CORNER_INDEX_CNT - 1 - i] * cnt;
  }
  return factorial_arr[cube::CORNER_INDEX_CNT] * rotation_coord + position_coord;
}

void CornerPatternDatabase::Calc() {
    for (size_t i = 0; i < distance_.Size(); ++i) {
        distance_.Set(i, 0x0F);
    }
    const auto start = cube::Cube();
    std::vector<cube::Cube> q = {start};
    distance_.Set(GetCornerCoordinate(start), 0);
 
    for (size_t i = 0; i < q.size(); ++i) {
      if (i == 0 || i % (distance_.Size() / 400) == 0) {
        std::cerr << "calcing corner pattern database" << i << "/" << distance_.Size() << std::endl;
      }
      const auto cur = q[i];
      for (int face = 0; face < 6; ++face) {
        for (int rotation = 1; rotation <= 3; ++rotation) {
          auto move = cube::GetMove(rotation, face);
          auto neighbor = cur;
          neighbor.Turn(move);
          if (distance_.Get(GetCornerCoordinate(neighbor)) != 0x0F) {
            continue;
          }
          q.push_back(neighbor);
          distance_.Set(GetCornerCoordinate(neighbor), distance_.Get(GetCornerCoordinate(cur)) + 1);
        }
      }
    }
}


size_t CornerPatternDatabase::Get(const cube::Cube& cube) const {
  return distance_.Get(GetCornerCoordinate(cube));
}

} // namespace solver
