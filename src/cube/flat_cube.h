#pragma once

#include <algorithm>
#include <array>
#include <cassert>

#include "cube.h"

template <size_t N> class FlatCube : public Cube<N> {
public:
  FlatCube();

  bool IsSolved() const override;

  void Turn(Move move) override;

  const std::array<std::array<uint8_t, N * N>, FACES_COUNT> &GetCube() const;

private:
  std::array<std::array<uint8_t, N * N>, FACES_COUNT> cube_;

  std::array<std::pair<uint8_t, size_t>, N> GetRow(uint8_t face, uint8_t row,
                                                   bool reversed) const;

  std::array<std::pair<uint8_t, size_t>, N> GetCol(uint8_t face, uint8_t col,
                                                   bool reversed) const;

  void Swap(const std::array<std::pair<uint8_t, size_t>, N> &first,
            const std::array<std::pair<uint8_t, size_t>, N> &second);

  void ClockwiseEdgeRotate(uint8_t face, uint8_t move_index);

  void ClockwiseFaceRotate(uint8_t face);

  void ClockwiseRotate(uint8_t face, uint8_t move_index);
};

template <size_t N> FlatCube<N>::FlatCube() : Cube<N>() {
  for (size_t i = 0; i < FACES_COUNT; ++i) {
    for (size_t j = 0; j < N * N; ++j) {
      cube_[i][j] = i;
    }
  }
}

template <size_t N> bool FlatCube<N>::IsSolved() const {
  for (size_t i = 0; i < FACES_COUNT; ++i) {
    for (size_t j = 0; j < N * N; ++j) {
      if (cube_[i][j] != i) {
        return false;
      }
    }
  }
  return true;
}

template <size_t N>
const std::array<std::array<uint8_t, N * N>, FACES_COUNT> &
FlatCube<N>::GetCube() const {
  return cube_;
}

template <size_t N>
std::array<std::pair<uint8_t, size_t>, N>
FlatCube<N>::GetRow(uint8_t face, uint8_t row, bool reversed) const {
  std::array<std::pair<uint8_t, size_t>, N> result;
  for (size_t i = 0; i < N; ++i) {
    result[i] = {face, row * N + i};
  }
  if (reversed) {
    std::reverse(result.begin(), result.end());
  }
  return result;
}

template <size_t N>
std::array<std::pair<uint8_t, size_t>, N>
FlatCube<N>::GetCol(uint8_t face, uint8_t col, bool reversed) const {
  std::array<std::pair<uint8_t, size_t>, N> result;
  for (size_t i = 0; i < N; ++i) {
    result[i] = {face, i * N + col};
  }
  if (reversed) {
    std::reverse(result.begin(), result.end());
  }
  return result;
}

template <size_t N>
void FlatCube<N>::Swap(
    const std::array<std::pair<uint8_t, size_t>, N> &first,
    const std::array<std::pair<uint8_t, size_t>, N> &second) {
  for (int i = 0; i < N; ++i) {
    std::swap(cube_[first[i].first][first[i].second],
              cube_[second[i].first][second[i].second]);
  }
}

template <size_t N> void FlatCube<N>::Turn(Move move) {
  for (int i = 0; i < MoveUtils::getRotation(move); ++i) {
    ClockwiseRotate(MoveUtils::GetFace(move), MoveUtils::GetMoveIndex(move));
  }
}

template <size_t N>
void FlatCube<N>::ClockwiseRotate(uint8_t face, uint8_t move_index) {
  if (move_index == 0) {
    ClockwiseFaceRotate(face);
  }
  ClockwiseEdgeRotate(face, move_index);
}

template <size_t N> void FlatCube<N>::ClockwiseFaceRotate(uint8_t face) {
  std::array<uint8_t, N * N> new_face;
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < N; ++j) {
      new_face[j * N + (N - 1 - i)] = cube_[face][i * N + j];
    }
  }
  cube_[face] = std::move(new_face);
}

template <size_t N>
void FlatCube<N>::ClockwiseEdgeRotate(uint8_t face, uint8_t move_index) {
  assert(face < FACES_COUNT);
  std::array<std::array<std::pair<uint8_t, size_t>, N>, 4> rotate_edges;
  switch (static_cast<Face>(face)) {
  case Face::U:
    rotate_edges[0] =
        GetCol(static_cast<uint8_t>(Face::L), N - 1 - move_index, true);
    rotate_edges[1] =
        GetRow(static_cast<uint8_t>(Face::B), N - 1 - move_index, false);
    rotate_edges[2] = GetCol(static_cast<uint8_t>(Face::R), move_index, false);
    rotate_edges[3] = GetRow(static_cast<uint8_t>(Face::F), move_index, true);
    break;
  case Face::D:
    rotate_edges[0] =
        GetCol(static_cast<uint8_t>(Face::R), N - 1 - move_index, true);
    rotate_edges[1] = GetRow(static_cast<uint8_t>(Face::B), move_index, true);
    rotate_edges[2] = GetCol(static_cast<uint8_t>(Face::L), move_index, false);
    rotate_edges[3] =
        GetRow(static_cast<uint8_t>(Face::F), N - 1 - move_index, false);
    break;
  case Face::F:
    rotate_edges[0] =
        GetRow(static_cast<uint8_t>(Face::U), N - 1 - move_index, false);
    rotate_edges[1] =
        GetRow(static_cast<uint8_t>(Face::R), N - 1 - move_index, false);
    rotate_edges[2] =
        GetRow(static_cast<uint8_t>(Face::D), N - 1 - move_index, false);
    rotate_edges[3] =
        GetRow(static_cast<uint8_t>(Face::L), N - 1 - move_index, false);
    break;
  case Face::B:
    rotate_edges[0] = GetRow(static_cast<uint8_t>(Face::U), move_index, true);
    rotate_edges[1] = GetRow(static_cast<uint8_t>(Face::L), move_index, true);
    rotate_edges[2] = GetRow(static_cast<uint8_t>(Face::D), move_index, true);
    rotate_edges[3] = GetRow(static_cast<uint8_t>(Face::R), move_index, true);
    break;
  case Face::L:
    rotate_edges[0] = GetCol(static_cast<uint8_t>(Face::U), move_index, false);
    rotate_edges[1] = GetCol(static_cast<uint8_t>(Face::F), move_index, false);
    rotate_edges[2] =
        GetCol(static_cast<uint8_t>(Face::D), N - 1 - move_index, true);
    rotate_edges[3] = GetCol(static_cast<uint8_t>(Face::B), move_index, false);
    break;
  case Face::R:
    rotate_edges[0] =
        GetCol(static_cast<uint8_t>(Face::U), N - 1 - move_index, true);
    rotate_edges[1] =
        GetCol(static_cast<uint8_t>(Face::B), N - 1 - move_index, true);
    rotate_edges[2] = GetCol(static_cast<uint8_t>(Face::D), move_index, false);
    rotate_edges[3] =
        GetCol(static_cast<uint8_t>(Face::F), N - 1 - move_index, true);
    break;
  }
  Swap(rotate_edges[0], rotate_edges[1]);
  Swap(rotate_edges[2], rotate_edges[3]);
  Swap(rotate_edges[0], rotate_edges[2]);
}
