#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>

namespace utils {

class PackedArray {
public:
    enum class HugePageSize { Huge1GB, Huge2MB, Normal };

    PackedArray(size_t n, unsigned int bits_per_element = 4);
    ~PackedArray();

    PackedArray(const PackedArray&) = delete;
    PackedArray& operator=(const PackedArray&) = delete;

    PackedArray(PackedArray&& other) noexcept;
    PackedArray& operator=(PackedArray&& other) noexcept;

    void Set(size_t index, uint8_t value);
    uint8_t Get(size_t index) const;
    size_t Size() const { return n_; }
    void WriteToFile(const std::string& filename) const;
    void ReadFromFile(const std::string& filename);

private:
    size_t n_;
    unsigned int bits_per_element_;
    size_t data_size_;
    uint8_t* data_;
    size_t allocated_size_;

    bool TryAllocate(HugePageSize page_size);
};

}  // namespace utils
