#include "cube/move.h"
#include "cube/cube.h"
#include "presentation/cube_presentation.h"

int main() {
  cube::Cube cube{};
  presentation::PrintCube(cube.GetCubeLayout());
  // std::string move_string = "R U R' U'";
  // for (int i = 0; i < 5; ++i) {
  //   move_string += " R U R' U'";
  // }
  // std::vector<cube::Move> moves = cube::GetMovesFromString(move_string);
  // for (auto move : moves) {
  //   cube.Turn(move);
  //   presentation::PrintCube(cube.GetCubeLayout());
  // }
  return 0;
}