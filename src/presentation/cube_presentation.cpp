#include "cube_presentation.h"

#include <cmath>
#include <iostream>
#include <string>

namespace presentation {

const std::string RESET = "\033[0m";
const std::string BLACK = "\033[30m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";
const std::string MAGENTA = "\033[35m";
const std::string CYAN = "\033[36m";
const std::string WHITE = "\033[37m";
const std::string ORANGE = "\033[38;5;208m";

std::string FaceToColor(uint8_t face) {
  std::string cube_string = std::to_string(face);
  switch (face) {
  case cube::Face::U:
    return WHITE + cube_string;
    break;
  case cube::Face::D:
    return YELLOW + cube_string;
    break;
  case cube::Face::F:
    return RED + cube_string;
    break;
  case cube::Face::B:
    return ORANGE + cube_string;
    break;
  case cube::Face::L:
    return GREEN + cube_string;
    break;
  case cube::Face::R:
    return BLUE + cube_string;
    break;
  }
  return "";
}

void PrintCube(const cube::CubeLayout &cube_layout) {
  const size_t n = std::sqrt(cube_layout[0].size());
  std::vector<std::vector<std::string>> a(3 * n,
                                          std::vector<std::string>(4 * n, ""));
  for (int i = 0; i < 3 * n; ++i) {
    for (int j = 0; j < 4 * n; ++j) {
      a[i][j] = " ";
    }
  }
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      a[i + 0 * n][j + 1 * n] =
          FaceToColor(cube_layout[cube::Face::B][i * n + j]);
      a[i + 1 * n][j + 0 * n] =
          FaceToColor(cube_layout[cube::Face::L][i * n + j]);
      a[i + 1 * n][j + 1 * n] =
          FaceToColor(cube_layout[cube::Face::U][i * n + j]);
      a[i + 1 * n][j + 2 * n] =
          FaceToColor(cube_layout[cube::Face::R][i * n + j]);
      a[i + 1 * n][j + 3 * n] =
          FaceToColor(cube_layout[cube::Face::D][i * n + j]);
      a[i + 2 * n][j + 1 * n] =
          FaceToColor(cube_layout[cube::Face::F][i * n + j]);
    }
  }
  for (int i = 0; i < 3 * n; ++i) {
    for (int j = 0; j < 4 * n; ++j) {
      std::cout << a[i][j] << RESET << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

} // namespace presentation