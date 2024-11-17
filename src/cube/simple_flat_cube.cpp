#include "simple_flat_cube.h"

#include <algorithm>
#include <cassert>
#include <stdexcept>

namespace cube {

namespace {

std::vector<CubeLayout> GetAllRotates(SimpleFlatCube cube) {
  // TODO: change to x, x', y, y', z, z' rotates
  std::vector<CubeLayout> result;
  for (size_t i = 0; i < 4; ++i) {
    for (size_t j = 0; j < 4; ++j) {
      for (size_t k = 0; k < 4; ++k) {
        for (size_t l = 0; l < cube.GetSize() / 2; ++l) {
          cube.Turn(GetMove(l, i, Face::U));
          cube.Turn(GetMove(l, 4 - i, Face::D));

          cube.Turn(GetMove(l, j, Face::L));
          cube.Turn(GetMove(l, 4 - j, Face::R));

          cube.Turn(GetMove(l, k, Face::F));
          cube.Turn(GetMove(l, 4 - k, Face::B));
        }

        auto cube_layout = cube.GetCubeLayout();

        bool already_found = false;
        for (const auto &to : result) {
          if (cube_layout == to) {
            already_found = true;
            break;
          }
        }
        if (!already_found) {
          result.push_back(std::move(cube_layout));
        }
        for (size_t l = 0; l < cube.GetSize() / 2; ++l) {
          cube.Turn(GetMove(l, 4 - k, Face::F));
          cube.Turn(GetMove(l, k, Face::B));

          cube.Turn(GetMove(l, 4 - j, Face::L));
          cube.Turn(GetMove(l, j, Face::R));

          cube.Turn(GetMove(l, 4 - i, Face::U));
          cube.Turn(GetMove(l, i, Face::D));
        }
      }
    }
  }
  return result;
}

size_t CalculateHash(const CubeLayout &cube) {
  std::hash<uint8_t> hasher;
  size_t seed = 0;

  for (const auto &face : cube) {
    for (uint8_t element : face) {
      seed ^= hasher(element) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
  }

  return seed;
}

void Swap(size_t /*n*/, CubeLayout &cube,
          const std::vector<std::pair<uint8_t, size_t>> &first,
          const std::vector<std::pair<uint8_t, size_t>> &second) {
  assert(first.size() == second.size());
  for (int i = 0; i < first.size(); ++i) {
    std::swap(cube[first[i].first][first[i].second],
              cube[second[i].first][second[i].second]);
  }
}

std::vector<std::pair<uint8_t, size_t>>
GetRow(size_t n, CubeLayout &cube, uint8_t face, uint8_t row, bool reversed) {
  std::vector<std::pair<uint8_t, size_t>> result(n);
  for (size_t i = 0; i < n; ++i) {
    result[i] = {face, row * n + i};
  }
  if (reversed) {
    std::reverse(result.begin(), result.end());
  }
  return result;
}

std::vector<std::pair<uint8_t, size_t>>
GetCol(size_t n, CubeLayout &cube, uint8_t face, uint8_t col, bool reversed) {
  std::vector<std::pair<uint8_t, size_t>> result(n);
  for (size_t i = 0; i < n; ++i) {
    result[i] = {face, i * n + col};
  }
  if (reversed) {
    std::reverse(result.begin(), result.end());
  }
  return result;
}

void ClockwiseEdgeRotate(size_t n, CubeLayout &cube, uint8_t face,
                         uint8_t move_index) {
  assert(face < FACES_COUNT);
  std::array<std::vector<std::pair<uint8_t, size_t>>, 4> rotate_edges;
  switch (face) {
  case Face::U:
    rotate_edges[0] = GetCol(n, cube, Face::L, n - 1 - move_index, true);
    rotate_edges[1] = GetRow(n, cube, Face::B, n - 1 - move_index, false);
    rotate_edges[2] = GetCol(n, cube, Face::R, move_index, false);
    rotate_edges[3] = GetRow(n, cube, Face::F, move_index, true);
    break;
  case Face::D:
    rotate_edges[0] = GetCol(n, cube, Face::R, n - 1 - move_index, true);
    rotate_edges[1] = GetRow(n, cube, Face::B, move_index, true);
    rotate_edges[2] = GetCol(n, cube, Face::L, move_index, false);
    rotate_edges[3] = GetRow(n, cube, Face::F, n - 1 - move_index, false);
    break;
  case Face::F:
    rotate_edges[0] = GetRow(n, cube, Face::U, n - 1 - move_index, false);
    rotate_edges[1] = GetRow(n, cube, Face::R, n - 1 - move_index, false);
    rotate_edges[2] = GetRow(n, cube, Face::D, n - 1 - move_index, false);
    rotate_edges[3] = GetRow(n, cube, Face::L, n - 1 - move_index, false);
    break;
  case Face::B:
    rotate_edges[0] = GetRow(n, cube, Face::U, move_index, true);
    rotate_edges[1] = GetRow(n, cube, Face::L, move_index, true);
    rotate_edges[2] = GetRow(n, cube, Face::D, move_index, true);
    rotate_edges[3] = GetRow(n, cube, Face::R, move_index, true);
    break;
  case Face::L:
    rotate_edges[0] = GetCol(n, cube, Face::U, move_index, false);
    rotate_edges[1] = GetCol(n, cube, Face::F, move_index, false);
    rotate_edges[2] = GetCol(n, cube, (Face::D), n - 1 - move_index, true);
    rotate_edges[3] = GetCol(n, cube, Face::B, move_index, false);
    break;
  case Face::R:
    rotate_edges[0] = GetCol(n, cube, Face::U, n - 1 - move_index, true);
    rotate_edges[1] = GetCol(n, cube, Face::B, n - 1 - move_index, true);
    rotate_edges[2] = GetCol(n, cube, (Face::D), move_index, false);
    rotate_edges[3] = GetCol(n, cube, Face::F, n - 1 - move_index, true);
    break;
  }
  Swap(n, cube, rotate_edges[0], rotate_edges[1]);
  Swap(n, cube, rotate_edges[2], rotate_edges[3]);
  Swap(n, cube, rotate_edges[0], rotate_edges[2]);
}

void ClockwiseFaceRotate(size_t n, CubeLayout &cube, uint8_t face) {
  std::vector<uint8_t> new_face(n * n);
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < n; ++j) {
      new_face[j * n + (n - 1 - i)] = cube[face][i * n + j];
    }
  }
  cube[face] = std::move(new_face);
}

void ClockwiseRotate(size_t n, CubeLayout &cube, uint8_t face,
                     uint8_t move_index) {
  if (move_index == 0) {
    ClockwiseFaceRotate(n, cube, face);
  }
  ClockwiseEdgeRotate(n, cube, face, move_index);
}

} // namespace

SimpleFlatCube::SimpleFlatCube(size_t n) : cube::Cube(n) {
  for (size_t i = 0; i < FACES_COUNT; ++i) {
    cube_[i].resize(n_ * n_);
    for (size_t j = 0; j < n_ * n_; ++j) {
      cube_[i][j] = i;
    }
  }
}

bool SimpleFlatCube::IsSolved() const {
  for (size_t i = 0; i < FACES_COUNT; ++i) {
    for (size_t j = 0; j < n_ * n_; ++j) {
      if (cube_[i][j] != i) {
        return false;
      }
    }
  }
  return false;
}

void SimpleFlatCube::Turn(Move move) {
  cube_layouts_.clear();
  for (int i = 0; i < getRotation(move); ++i) {
    ClockwiseRotate(n_, cube_, GetFace(move), GetMoveIndex(move));
  }
}

const CubeLayout &SimpleFlatCube::GetCubeLayout() const { return cube_; }

size_t SimpleFlatCube::GetHash() const {
  if (n_ % 2) {
    return CalculateHash(cube_);
  }
  if (cube_layouts_.empty()) {
    cube_layouts_ = GetAllRotates(*this);
  }

  size_t result = CalculateHash(cube_);
  for (const auto &layout : cube_layouts_) {
    size_t cur_result = CalculateHash(layout);
    if (cur_result < result) {
      result = cur_result;
    }
  }
  return result;
}

bool SimpleFlatCube::operator==(const SimpleFlatCube &other) const {
  if (n_ % 2) {
    return GetCubeLayout() == other.GetCubeLayout();
  }

  if (cube_layouts_.empty()) {
    cube_layouts_ = GetAllRotates(*this);
  }
  for (const auto &layout : cube_layouts_) {
    if (layout == other.GetCubeLayout()) {
      return true;
    }
  }
  return false;
}

} // namespace cube