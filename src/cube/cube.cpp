#include "cube.h"

#include <algorithm>
#include <cassert>
#include <stdexcept>

namespace cube {

void Cube::ClockwiseEdgeRotate(uint8_t face) {
  assert(face < FACES_COUNT);
  std::array<std::vector<std::pair<uint8_t, size_t>>, 4> rotate_edges;
  switch (face) {
  case Face::U:
    rotate_edges[0] = GetCol(Face::L, CUBE_SIZE - 1, true);
    rotate_edges[1] = GetRow(Face::B, CUBE_SIZE - 1, false);
    rotate_edges[2] = GetCol(Face::R, 0, false);
    rotate_edges[3] = GetRow(Face::F, 0, true);
    break;
  case Face::D:
    rotate_edges[0] = GetCol(Face::R, CUBE_SIZE - 1, true);
    rotate_edges[1] = GetRow(Face::B, 0, true);
    rotate_edges[2] = GetCol(Face::L, 0, false);
    rotate_edges[3] = GetRow(Face::F, CUBE_SIZE - 1, false);
    break;
  case Face::F:
    rotate_edges[0] = GetRow(Face::U, CUBE_SIZE - 1, false);
    rotate_edges[1] = GetRow(Face::R, CUBE_SIZE - 1, false);
    rotate_edges[2] = GetRow(Face::D, CUBE_SIZE - 1, false);
    rotate_edges[3] = GetRow(Face::L, CUBE_SIZE - 1, false);
    break;
  case Face::B:
    rotate_edges[0] = GetRow(Face::U, 0, true);
    rotate_edges[1] = GetRow(Face::L, 0, true);
    rotate_edges[2] = GetRow(Face::D, 0, true);
    rotate_edges[3] = GetRow(Face::R, 0, true);
    break;
  case Face::L:
    rotate_edges[0] = GetCol(Face::U, 0, false);
    rotate_edges[1] = GetCol(Face::F, 0, false);
    rotate_edges[2] = GetCol(Face::D, CUBE_SIZE - 1, true);
    rotate_edges[3] = GetCol(Face::B, 0, false);
    break;
  case Face::R:
    rotate_edges[0] = GetCol(Face::U, CUBE_SIZE - 1, true);
    rotate_edges[1] = GetCol(Face::B, CUBE_SIZE - 1, true);
    rotate_edges[2] = GetCol(Face::D, 0, false);
    rotate_edges[3] = GetCol(Face::F, CUBE_SIZE - 1, true);
    break;
  }
  Swap(rotate_edges[0], rotate_edges[1]);
  Swap(rotate_edges[2], rotate_edges[3]);
  Swap(rotate_edges[0], rotate_edges[2]);
}

void Cube::ClockwiseFaceRotate(uint8_t face) {
  std::vector<uint8_t> new_face(CUBE_SIZE * CUBE_SIZE);
  for (size_t i = 0; i < CUBE_SIZE; ++i) {
    for (size_t j = 0; j < CUBE_SIZE; ++j) {
      new_face[j * CUBE_SIZE + (CUBE_SIZE - 1 - i)] =
          cube_[face][i * CUBE_SIZE + j];
    }
  }
  cube_[face] = std::move(new_face);
  for (size_t i = 0; i < CUBE_SIZE; ++i) {
    for (size_t j = 0; j < CUBE_SIZE; ++j) {
    }
  }
}

std::vector<std::pair<uint8_t, size_t>> Cube::GetRow(uint8_t face, uint8_t row,
                                                     bool reversed) const {
  std::vector<std::pair<uint8_t, size_t>> result(CUBE_SIZE);
  for (size_t i = 0; i < CUBE_SIZE; ++i) {
    result[i] = {face, row * CUBE_SIZE + i};
  }
  if (reversed) {
    std::reverse(result.begin(), result.end());
  }
  return result;
}

std::vector<std::pair<uint8_t, size_t>> Cube::GetCol(uint8_t face, uint8_t col,
                                                     bool reversed) const {
  std::vector<std::pair<uint8_t, size_t>> result(CUBE_SIZE);
  for (size_t i = 0; i < CUBE_SIZE; ++i) {
    result[i] = {face, i * CUBE_SIZE + col};
  }
  if (reversed) {
    std::reverse(result.begin(), result.end());
  }
  return result;
}

void Cube::Swap(const std::vector<std::pair<uint8_t, size_t>> &first,
                const std::vector<std::pair<uint8_t, size_t>> &second) {
  assert(first.size() == second.size());
  for (int i = 0; i < first.size(); ++i) {
    std::swap(cube_[first[i].first][first[i].second],
              cube_[second[i].first][second[i].second]);
  }
}

Cube::Cube() {
  for (size_t i = 0; i < FACES_COUNT; ++i) {
    cube_[i].resize(CUBE_SIZE * CUBE_SIZE);
    for (size_t j = 0; j < CUBE_SIZE * CUBE_SIZE; ++j) {
      cube_[i][j] = i;
    }
  }
}

bool Cube::IsSolved() const {
  for (size_t i = 0; i < FACES_COUNT; ++i) {
    for (size_t j = 0; j < CUBE_SIZE * CUBE_SIZE; ++j) {
      if (cube_[i][j] != i) {
        return false;
      }
    }
  }
  return true;
}

void Cube::Turn(Move move) {
  for (int i = 0; i < getRotation(move); ++i) {
    ClockwiseFaceRotate(GetFace(move));
    ClockwiseEdgeRotate(GetFace(move));
  }
}

const CubeLayout &Cube::GetCubeLayout() const { return cube_; }

size_t Cube::GetHash() const {
  std::hash<uint8_t> hasher;
  size_t seed = 0;

  for (const auto &face : cube_) {
    for (uint8_t element : face) {
      seed ^= hasher(element) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
  }

  return seed;
}

bool Cube::operator==(const Cube &other) const {
  return GetCubeLayout() == other.GetCubeLayout();
}

bool Cube::operator!=(const Cube &other) const {
  return GetCubeLayout() != other.GetCubeLayout();
}


} // namespace cube