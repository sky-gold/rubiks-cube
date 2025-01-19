#pragma once

#include "../cube/cube.h"

#include <fstream>
#include <stdexcept>

namespace solver {

constexpr uint64_t factorial(size_t n) {
  return (n <= 1) ? 1 : n * factorial(n - 1);
}

constexpr uint64_t power(uint64_t base, size_t exponent) {
  return (exponent == 0) ? 1 : base * power(base, exponent - 1);
}

constexpr uint64_t CORNER_PATTERN_SIZE = factorial(8) * power(3, 7);

template <size_t N> class PackedArray {
public:
  PackedArray() : data((N + 1) / 2, 0) {}

  void Set(size_t index, uint8_t value) {
    if (index >= N) {
      throw std::out_of_range("Index out of range in PackedArray::Set");
    }
    value &= 0x0F;
    size_t byte_index = index / 2;
    size_t bit_offset = (index % 2) * 4;
    data[byte_index] &= ~(0x0F << bit_offset);
    data[byte_index] |= (value << bit_offset);
  }

  uint8_t Get(size_t index) const {
    if (index >= N) {
      throw std::out_of_range("Index out of range in PackedArray::Get");
    }
    size_t byte_index = index / 2;
    size_t bit_offset = (index % 2) * 4;
    return (data[byte_index] >> bit_offset) & 0x0F;
  }

  constexpr size_t Size() const { return N; }

  void WriteToFile(const std::string &filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (file.fail()) {
      throw std::runtime_error("Failed to open file for writing: " + filename);
    }
    file.write(reinterpret_cast<const char *>(data.data()), data.size());
    if (file.fail()) {
      throw std::runtime_error("Failed to write data to file: " + filename);
    }
  }

  void ReadFromFile(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary);
    if (file.fail()) {
      throw std::runtime_error("Failed to open file for reading: " + filename);
    }
    file.read(reinterpret_cast<char *>(data.data()), data.size());
    if (file.fail()) {
      throw std::runtime_error("Failed to read data from file: " + filename);
    }
  }

private:
  std::vector<uint8_t> data;
};

class CornerPatternDatabase {
public:
  CornerPatternDatabase();

  size_t Get(const cube::Cube &cube) const;

private:
  void LoadOrCalcAndWrite();

  void Calc();

  size_t GetCornerCoordinate(const cube::Cube &cube) const;

  const std::string filename_ = "corner_pattern_database.bin";

  PackedArray<CORNER_PATTERN_SIZE> distance_;
};

} // namespace solver
