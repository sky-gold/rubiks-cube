#include "solver.h"

#include <cassert>
#include <future>
#include <iostream>
#include <limits>
#include <algorithm>

namespace solver {

namespace {

std::vector<cube::Move> GenerateAllMoves() {
    std::vector<cube::Move> moves;
    for (int face = 0; face < 6; ++face) {
        for (int rotation = 1; rotation <= 3; ++rotation) {
            moves.push_back(cube::GetMove(rotation, face));
        }
    }
    return moves;
}

std::vector<std::pair<size_t, size_t>> DivideTasks(size_t n, size_t num_threads) {
    std::vector<std::pair<size_t, size_t>> result;
    size_t base = n / num_threads;
    size_t remainder = n % num_threads;

    size_t last_pos = 0;
    for (size_t i = 0; i < num_threads; ++i) {
        if (i < remainder) {
            result.push_back({last_pos, last_pos + base + 1});
        } else {
            result.push_back({last_pos, last_pos + base});
        }
        last_pos = result.back().second;
    }
    return result;
}

constexpr size_t INF = std::numeric_limits<size_t>::max();

}  // namespace

Solver::Solver(heuristic::HeuristicFunction&& h_func, size_t num_threads) : h_func_(std::move(h_func)), num_threads_(num_threads) {};

std::optional<std::vector<cube::Move>> Solver::Solve(cube::Cube start) {
    size_t bound = h_func_.GetHeuristic(start);
    shared_result_.reset();
    found_solution_.store(false);

    while (true) {
        size_t new_bound = ParallelSearch(start, bound);
        if (found_solution_.load(std::memory_order_relaxed)) {
            std::lock_guard<std::mutex> lock(result_mutex_);
            return shared_result_;
        }
        if (new_bound == INF) {
            return std::nullopt;
        }
        bound = new_bound;
    }
}

size_t Solver::ParallelSearch(const cube::Cube& start, size_t bound) {
    std::vector<std::pair<size_t, cube::Move>> sorted_moves;
    sorted_moves.reserve(18);

    for (int face = 0; face < 6; ++face) {
        for (int rotation = 1; rotation <= 3; ++rotation) {
            auto move = cube::GetMove(rotation, face);
            cube::Cube local_cube = start;
            local_cube.Turn(move);
            sorted_moves.push_back({h_func_.GetHeuristic(local_cube), move});
        }
    }

    std::sort(sorted_moves.begin(), sorted_moves.end());

    std::atomic<size_t> current_index{0};
    std::vector<std::future<size_t>> futures;

    for (size_t i = 0; i < num_threads_; ++i) {
        futures.emplace_back(std::async(std::launch::async, [this, start, bound, &current_index, &sorted_moves]() {
            size_t local_min_bound = INF;

            while (true) {
                size_t j = current_index.fetch_add(1, std::memory_order_acquire);
                
                if (j >= sorted_moves.size() || found_solution_.load(std::memory_order_acquire)) {
                    break;
                }

                cube::Cube local_cube = start;
                auto move = sorted_moves[j].second;
                local_cube.Turn(move);
                std::vector<cube::Move> local_path = {move};
                size_t current_bound = this->Search(local_cube, local_path, 1, bound);

                if (current_bound < local_min_bound) {
                    local_min_bound = current_bound;
                }
            }

            return local_min_bound;
        }));
    }

    size_t global_min_bound = INF;
    for (auto& future : futures) {
        size_t current_bound = future.get();
        if (current_bound < global_min_bound) {
            global_min_bound = current_bound;
        }
    }

    return global_min_bound;
}

size_t Solver::Search(cube::Cube& cube, std::vector<cube::Move>& path, size_t distance, size_t bound, std::optional<size_t> maybe_h ) {
    if (found_solution_.load(std::memory_order_relaxed)) {
        return 0;
    }

    if (cube.IsSolved()) {
        std::lock_guard<std::mutex> lock(result_mutex_);
        if (!shared_result_.has_value()) {
            shared_result_ = path;
            found_solution_.store(true, std::memory_order_relaxed);
        }
        return 0;
    }
    size_t h = maybe_h.has_value() ? maybe_h.value() : h_func_.GetHeuristic(cube);
    size_t f = distance + h;
    if (f > bound) {
        return f;
    }

    size_t min = INF;
    uint8_t repeated_face = 7;
    if (!path.empty()) {
        repeated_face = cube::GetFace(path.back());
    }

    uint8_t repeated_opposite_face = 7;
    if (path.size() >= 2 &&
        cube::GetFace(path[path.size() - 2]) == cube::GetOppositeFace(cube::GetFace(path[path.size() - 1]))) {
        repeated_opposite_face = cube::GetFace(path[path.size() - 2]);
    }

    if (!path.empty() &&
        cube::GetOppositeFace(cube::GetFace(path[path.size() - 1])) < cube::GetFace(path[path.size() - 1])) {
        repeated_opposite_face = cube::GetOppositeFace(cube::GetFace(path[path.size() - 1]));
    }

    std::vector<std::pair<size_t, cube::Move>> moves;
    moves.reserve(15);

    for (int face = 0; face < 6; ++face) {
        if (face == repeated_face || face == repeated_opposite_face) {
            continue;
        }
        for (int rotation = 1; rotation <= 3; ++rotation) {
            auto move = cube::GetMove(rotation, face);
            cube.Turn(move);
            moves.push_back({h_func_.GetHeuristic(cube), move});
            cube.Turn(cube::GetAntiMove(move));
        }
    }

    std::sort(moves.begin(), moves.end());

    for (auto [next_h, move]: moves) {
        path.push_back(move);
        cube.Turn(move);
        size_t node_bound = Search(cube, path, distance + 1, bound, next_h);

        if (node_bound < min) {
            min = node_bound;
        }
        cube.Turn(cube::GetAntiMove(move));
        path.pop_back();
    }

    return min;
}

}  // namespace solver
