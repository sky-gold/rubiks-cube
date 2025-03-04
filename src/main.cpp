#include "cube/cube.h"
#include "cube/move.h"
#include "heuristic/heuristic_function.h"
#include "solver/solver.h"

#include <iostream>
#include <string>

cube::Cube getCubeFromString(const std::string &str) {
  cube::Cube cube;
  for (auto move : cube::GetMovesFromString(str)) {
    cube.Turn(move);
  }
  return cube;
}

int main() {
  auto heuristic_function = heuristic::HeuristicFunction();
  auto solver = solver::Solver(std::move(heuristic_function));
  std::string line;
  while (std::getline(std::cin, line)) {
    auto cube = getCubeFromString(line);
    auto solution = solver.Solve(cube);
    std::cout << cube::MovesToString(solution.value()) << std::endl;
  }
  return 0;
}