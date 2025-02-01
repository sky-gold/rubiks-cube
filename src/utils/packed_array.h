#pragma once

#include <cstdint>
#include <vector>
#include <string>

namespace utils {

class PackedArray {
public:
  PackedArray(size_t n);

  void Set(size_t index, uint8_t value);

  uint8_t Get(size_t index) const;

  size_t Size() const;

  void WriteToFile(const std::string &filename) const;

  void ReadFromFile(const std::string &filename);

private:
  size_t n_;
  std::vector<uint8_t> data_;
};

} // namespace utils
