#include "pattern_database_heuristic.h"

#include <chrono>
#include <iostream>
#include <iomanip>
#include <stack>
#include <stdexcept>
#include <deque>


namespace heuristic {

PatternDatabaseHeuristic::PatternDatabaseHeuristic(
    size_t base,
    unsigned int bits_per_element,
    std::unique_ptr<CoordinateCalculator>&& calculator,
    const std::string& filename,
    size_t max_value
)
    : base_(base),
      bits_per_element_(bits_per_element),
      calculator_(std::move(calculator)),
      filename_(filename),
      database_(std::make_unique<utils::PackedArray>(calculator_->Size(), bits_per_element)),
      max_value_(max_value) {
    try {
        database_->ReadFromFile(filename_);
        std::cerr << "[INFO] Database loaded from file: " << filename_ << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[WARNING] Failed to load database from file: " << filename_ << ". Creating a new one."
                  << std::endl;
        // CalcBFS();
        CalcBFSInplaceQueueless();
        database_->WriteToFile(filename);
    }
    // std::vector<size_t> cnt(21, 0);
    // for (size_t i = 0; i < database_->Size(); ++i) {
    //     auto value = database_->Get(i);
    //     ++cnt[value];
    //     if (value == 0x0F) {
    //         std::cerr << "[WARNING] find INF values in distance"<< std::endl;
    //         database_->Set(i, 0);
    //     } else if (value <= base_) {
    //         database_->Set(i, 0);
    //     } else {
    //         database_->Set(i, value - base_);
    //     }
    // }
    // for (size_t i = 0; i < cnt.size(); ++i) {
    //     std::cerr << "[INFO] cnt[" << i << "]=" << cnt[i] << std::endl;
    // }
}

size_t PatternDatabaseHeuristic::GetMaxValue() const {
    return max_value_;
}


size_t PatternDatabaseHeuristic::GetHeuristic(const cube::Cube& cube) const {
    size_t coordinate = calculator_->GetCoordinate(cube);
    size_t value = database_->Get(coordinate);

    return (value == 0) ? 0 : (base_ + value);
}


std::vector<size_t> PatternDatabaseHeuristic::GetNeighbors(size_t coord) {
    auto cube = calculator_->GetCoordinateCube(coord);
    std::vector<size_t> result;
    result.reserve(18);
    for (int face = 0; face < 6; ++face) {
        for (int rotation = 1; rotation <= 3; ++rotation) {
            const auto move = cube::GetMove(rotation, face);
            cube.Turn(move);
            result.push_back(calculator_->GetCoordinate(cube));
            cube.Turn(cube::GetAntiMove(move));
        }
    }
    return result;
}


void PatternDatabaseHeuristic::CalcBFSInplaceQueueless() {
    if (bits_per_element_ != 4 || base_ != 0) {
        throw std::runtime_error("bits_per_element_ != 4 || base_ != 0");
    }
    auto& distance = database_;
    for (size_t i = 0; i < distance->Size(); ++i) {
        distance->Set(i, 0x0F);
    }
    const auto start = cube::Cube();
    distance->Set(calculator_->GetCoordinate(start), 0);
    size_t cnt = 1;
    size_t cur_depth = 0;
    while (cnt < distance->Size()) {
        std::cerr << "[INFO] CalcBFSInplaceQueueless cur_depth=" << cur_depth << std::endl;
        for (size_t i = 0; i < distance->Size(); ++i) {
            if (i == 0 || i % (distance->Size() / 100) == 0) {
                std::cerr << "[INFO] CalcBFSInplaceQueueless cur_depth=" << cur_depth << " [" << ((i * 100) / distance->Size()) << "/100 of layer]" << std::endl;
            }
            if (distance->Get(i) != cur_depth) {
                continue;
            }
            for (auto neighbor: GetNeighbors(i)) {
                if (distance->Get(neighbor) != 0x0F) {
                    continue;
                }
                ++cnt;
                distance->Set(neighbor, cur_depth + 1);
            }
        }
        ++cur_depth;
    }
}

void PatternDatabaseHeuristic::CalcBFS() {
    auto distance = utils::PackedArray(calculator_->Size(), 4);
    for (size_t i = 0; i < distance.Size(); ++i) {
        distance.Set(i, 0x0F);
    }
    const auto start = cube::Cube();
    std::deque<size_t> q = {calculator_->GetCoordinate(start)};
    distance.Set(calculator_->GetCoordinate(start), 0);
    auto start_time = std::chrono::high_resolution_clock::now();
    size_t i = 0;
    while (!q.empty()) {
        if (i == 0 || i % (distance.Size() / 400) == 0) {
            std::cerr << "[INFO] CalcBFS " << i << "/" << distance.Size() << " [" << ((i * 100) / distance.Size()) << "%]" << std::endl;
            if (i > 0) {
                std::chrono::duration<double> elapsed_time = std::chrono::high_resolution_clock::now()- start_time;
                double avg_time_per_iter = elapsed_time.count() / i;
                double remaining_seconds = avg_time_per_iter * (distance.Size() - i);
                double remaining_minutes = remaining_seconds / 60;
                std::cerr << std::fixed << std::setprecision(1) << "[INFO] Remaining: ~" << remaining_minutes << " min" << std::endl;
            }
            std::cerr << "[INFO] Memory: " << distance.Size() / (2 * 1048576LL) << " mb" << std::endl;
            std::cerr << "[INFO] -------------------------------" << std::endl;
        }
        ++i;
        const auto cur_coord = q.front();
        const auto cur = calculator_->GetCoordinateCube(cur_coord);
        q.pop_front();
        const auto cur_distance = distance.Get(cur_coord);
        for (int face = 0; face < 6; ++face) {
            for (int rotation = 1; rotation <= 3; ++rotation) {
                auto move = cube::GetMove(rotation, face);
                auto neighbor = cur;
                neighbor.Turn(move);
                auto neighbor_coord = calculator_->GetCoordinate(neighbor);
                if (distance.Get(neighbor_coord) != 0x0F) {
                    continue;
                }
                q.push_back(neighbor_coord);
                auto neighbor_distance = cur_distance + 1;
                if (neighbor_distance == 0x0F) {
                    throw std::runtime_error("some of distance break max limit");
                }
                distance.Set(neighbor_coord, neighbor_distance);
            }
        }
    }
    std::cerr << "[INFO] Ending calc, checked " << q.size() << " positions" << std::endl;
    std::vector<size_t> cnt(21, 0);
    for (size_t i = 0; i < distance.Size(); ++i) {
        auto value = distance.Get(i);
        ++cnt[value];
        if (value == 0x0F) {
            std::cerr << "[WARNING] find INF values in distance"<< std::endl;
            database_->Set(i, 0);
        } else if (value <= base_) {
            database_->Set(i, 0);
        } else {
            database_->Set(i, value - base_);
        }
    }
    for (size_t i = 0; i < cnt.size(); ++i) {
        std::cerr << "[INFO] cnt[" << i << "]=" << cnt[i] << std::endl;
    }
}

// void PatternDatabaseHeuristic::CalcUsingIDDFS() {
//     auto data = DataDLS{
//         .distance = utils::PackedArray(calculator_->Size(), 4),
//         .found_new = false,
//         .total_processed = 0,
//         .max_depth = 0,
//         .path = {},
//         .cube = cube::Cube(),
//         .logged = false
//     };

//     for (size_t i = 0; i < data.distance.Size(); ++i) {
//         data.distance.Set(i, 0x0F);
//     }

//     auto start_time = std::chrono::high_resolution_clock::now();

//     while (true) {
//         std::cerr << "[INFO] max_depth=" << data.max_depth << std::endl;
        
//         data.found_new = false;

//         DLS(data, 0);

//         if (!data.found_new) break;

//         data.max_depth++;
//     }

//     std::cerr << "Max depth reached: " << data.max_depth << " | Total processed: " << data.total_processed << " | Time: "
//               << std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start_time).count() << "s\n";
    
//     for (size_t i = 0; i < data.distance.Size(); ++i) {
//         auto value = data.distance.Get(i);
//         if (value == 0x0F) {
//             std::cerr << "[WARNING] find INF values in distance"<< std::endl;
//             database_->Set(i, 0);
//         } else if (value <= base_) {
//             database_->Set(i, 0);
//         } else {
//             database_->Set(i, value - base_);
//         }
//     }
// }

// void PatternDatabaseHeuristic::DLS(DataDLS & data, size_t depth) {
//     const size_t coord = calculator_->GetCoordinate(data.cube);
//     const uint8_t cur_distance = data.distance.Get(coord);

//     if (data.total_processed == 0 || data.total_processed % (data.distance.Size() / 400) == 0) {
//         if (!data.logged) {
//             std::cerr << "[INFO] " << data.total_processed << " / " << data.distance.Size() << " ";
//             std::cerr << "(" << ((data.total_processed * 100) / data.distance.Size()) << "%)";
//             std::cerr << std::endl;
//             data.logged = true;
//         }
//     } else {
//         data.logged = false;
//     }
//     if (depth < cur_distance) {
//         data.distance.Set(coord, depth);
//         data.found_new = true;
//         ++data.total_processed;
//     }
//     if (depth == data.max_depth) {
//         return;
//     }
//     auto& path = data.path;
//     uint8_t repeated_face = 7;
//     if (!path.empty()) {
//         repeated_face = cube::GetFace(data.path.back());
//     }

//     uint8_t repeated_opposite_face = 7;
//     if (path.size() >= 2 &&
//         cube::GetFace(path[path.size() - 2]) == cube::GetOppositeFace(cube::GetFace(path[path.size() - 1]))) {
//         repeated_opposite_face = cube::GetFace(path[path.size() - 2]);
//     }

//     if (!path.empty() &&
//         cube::GetOppositeFace(cube::GetFace(path[path.size() - 1])) < cube::GetFace(path[path.size() - 1])) {
//         repeated_opposite_face = cube::GetOppositeFace(cube::GetFace(path[path.size() - 1]));
//     }

//     for (int face = 0; face < 6; ++face) {
//         if (face == repeated_face || face == repeated_opposite_face) {
//             continue;
//         }
//         for (int rotation = 1; rotation <= 3; ++rotation) {
//             auto move = cube::GetMove(rotation, face);
//             path.push_back(move);
//             data.cube.Turn(move);
//             DLS(data, depth + 1);
//             data.cube.Turn(cube::GetAntiMove(move));
//             path.pop_back();
//         }
//     }
// }

}  // namespace heuristic
