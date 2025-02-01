#include "packed_array.h"

#include <fstream>
#include <stdexcept>

namespace utils {

PackedArray::PackedArray(size_t n) : n_(n), data_((n + 1) / 2, 0) {}

void PackedArray::Set(size_t index, uint8_t value) {
  if (index >= n_) {
    throw std::out_of_range("Index out of range in PackedArray::Set");
  }
  value &= 0x0F;
  size_t byte_index = index / 2;
  size_t bit_offset = (index % 2) * 4;
  data_[byte_index] &= ~(0x0F << bit_offset);
  data_[byte_index] |= (value << bit_offset);
}

uint8_t PackedArray::Get(size_t index) const {
  if (index >= n_) {
    throw std::out_of_range("Index out of range in PackedArray::Get");
  }
  size_t byte_index = index / 2;
  size_t bit_offset = (index % 2) * 4;
  return (data_[byte_index] >> bit_offset) & 0x0F;
}

size_t PackedArray::Size() const { return n_; }

void PackedArray::WriteToFile(const std::string &filename) const {
  std::ofstream file(filename, std::ios::binary);
  if (file.fail()) {
    throw std::runtime_error("Failed to open file for writing: " + filename);
  }
  file.write(reinterpret_cast<const char *>(data_.data()), data_.size());
  if (file.fail()) {
    throw std::runtime_error("Failed to write data to file: " + filename);
  }
}

void PackedArray::ReadFromFile(const std::string &filename) {
  std::ifstream file(filename, std::ios::binary);
  if (file.fail()) {
    throw std::runtime_error("Failed to open file for reading: " + filename);
  }
  file.read(reinterpret_cast<char *>(data_.data()), data_.size());
  if (file.fail()) {
    throw std::runtime_error("Failed to read data from file: " + filename);
  }
}

} // namespace utils
