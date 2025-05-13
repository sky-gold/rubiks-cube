#include "cube/cube.h"
#include "cube/move.h"
#include "heuristic/heuristic_function.h"
#include "solver/solver.h"

#include <iostream>
#include <string>

cube::Cube getCubeFromString(const std::string& str) {
    cube::Cube cube;
    for (auto move : cube::GetMovesFromString(str)) {
        cube.Turn(move);
    }
    return cube;
}

int main(int argc, char* argv[]) {
    size_t threads_count = 8;
    auto pdb_size = heuristic::PDBSize::High;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--threads_count") {
            if (i + 1 < argc) {
                try {
                    threads_count = std::stoi(argv[i + 1]);
                    if (threads_count < 1) {
                        throw std::invalid_argument("");
                    }
                } catch (...) {
                    std::cerr << "Error: threads_count must be integer >= 1\n";
                    return 1;
                }
                ++i;
            } else {
                std::cerr << "Error: Missing value for --threads_count\n";
                return 1;
            }
        } else if (arg == "--pdb_size") {
            if (i + 1 < argc) {
                std::string pdb_size_str = argv[i + 1];
                if (pdb_size_str == "low") {
                    pdb_size = heuristic::PDBSize::Low;
                } else if (pdb_size_str == "mid") {
                    pdb_size = heuristic::PDBSize::Mid;
                } else if (pdb_size_str == "high") {
                    pdb_size = heuristic::PDBSize::High;
                } else {
                    std::cerr << "Error: pdb_size must be low, mid or high\n";
                    return 1;
                }
                ++i;
            } else {
                std::cerr << "Error: Missing value for --pdb_size\n";
                return 1;
            }
        } else {
            std::cerr << "Error: Unknown argument '" << arg << "'\n";
            return 1;
        }
    }
    auto heuristic_function = heuristic::HeuristicFunction(pdb_size);
    auto solver = solver::Solver(std::move(heuristic_function), threads_count);
    std::string line;
    while (std::getline(std::cin, line)) {
        auto cube = getCubeFromString(line);
        auto solution = solver.Solve(cube);
        std::cout << cube::MovesToString(solution.value()) << std::endl;
    }
    return 0;
}
