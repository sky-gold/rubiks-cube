#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>

#include "cube/flat_cube.h"
#include "cube/move_utils.h"

constexpr size_t N = 2;

char a[3 * N][4 * N];

void print_cube(const FlatCube<N> &flat_cube) {
  for (int i = 0; i < 3 * N; ++i) {
    for (int j = 0; j < 4 * N; ++j) {
      a[i][j] = ' ';
    }
  }
  const auto &cube = flat_cube.GetCube();
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      a[i][j + N] = '0' + cube[static_cast<size_t>(Face::B)][i * N + j];
      a[i + N][j] = '0' + cube[static_cast<size_t>(Face::L)][i * N + j];
      a[i + N][j + N] = '0' + cube[static_cast<size_t>(Face::U)][i * N + j];
      a[i + N][j + 2 * N] = '0' + cube[static_cast<size_t>(Face::R)][i * N + j];
      a[i + N][j + 3 * N] = '0' + cube[static_cast<size_t>(Face::D)][i * N + j];
      a[i + 2 * N][j + N] = '0' + cube[static_cast<size_t>(Face::F)][i * N + j];
    }
  }
  for (int i = 0; i < 3 * N; ++i) {
    for (int j = 0; j < 4 * N; ++j) {
      std::cout << a[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

int main() {
  FlatCube<N> flat_cube{};
  print_cube(flat_cube);
  flat_cube.Turn(MoveUtils::GetMove(0, 1, static_cast<uint8_t>(Face::U)));
  std::cout << std::endl;
  print_cube(flat_cube);

  flat_cube.Turn(MoveUtils::GetMove(0, 1, static_cast<uint8_t>(Face::R)));
  std::cout << std::endl;
  print_cube(flat_cube);
  return 0;
}