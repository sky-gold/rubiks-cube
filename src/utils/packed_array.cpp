#include "packed_array.h"
#include <sys/mman.h>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <system_error>

namespace utils {

PackedArray::PackedArray(size_t n, unsigned int bits_per_element)
    : n_(n), bits_per_element_(bits_per_element), data_size_(0), data_(nullptr), allocated_size_(0) {
    if (bits_per_element_ != 2 && bits_per_element_ != 4 && bits_per_element_ != 8) {
        throw std::invalid_argument("bits_per_element must be 2, 4, or 8");
    }

    data_size_ = (n * bits_per_element_ + 7) / 8;

    if (n_ == 0) {
        return;
    }

    if (!TryAllocate(HugePageSize::Huge1GB)) {
        if (!TryAllocate(HugePageSize::Huge2MB)) {
            if (!TryAllocate(HugePageSize::Normal)) {
                throw std::bad_alloc();
            }
        }
    }
}

PackedArray::~PackedArray() {
    if (data_) {
        munmap(data_, allocated_size_);
    }
}

PackedArray::PackedArray(PackedArray&& other) noexcept
    : n_(other.n_),
      bits_per_element_(other.bits_per_element_),
      data_size_(other.data_size_),
      data_(other.data_),
      allocated_size_(other.allocated_size_) {
    other.data_ = nullptr;
    other.allocated_size_ = 0;
    other.n_ = 0;
    other.data_size_ = 0;
    other.bits_per_element_ = 4;
}

PackedArray& PackedArray::operator=(PackedArray&& other) noexcept {
    if (this != &other) {
        if (data_) {
            munmap(data_, allocated_size_);
        }
        n_ = other.n_;
        bits_per_element_ = other.bits_per_element_;
        data_size_ = other.data_size_;
        data_ = other.data_;
        allocated_size_ = other.allocated_size_;

        other.data_ = nullptr;
        other.allocated_size_ = 0;
        other.n_ = 0;
        other.data_size_ = 0;
        other.bits_per_element_ = 4;
    }
    return *this;
}

bool PackedArray::TryAllocate(HugePageSize page_size) {
    size_t page_bits = 0;
    bool use_huge = false;

    switch (page_size) {
        case HugePageSize::Huge1GB:
            page_bits = 30;
            use_huge = true;
            break;
        case HugePageSize::Huge2MB:
            page_bits = 21;
            use_huge = true;
            break;
        case HugePageSize::Normal:
            page_bits = 0;
            use_huge = false;
            break;
        default:
            return false;
    }

    size_t page_bytes = (page_bits == 0) ? 1 : (1ULL << page_bits);
    size_t num_pages = (data_size_ + page_bytes - 1) / page_bytes;
    size_t length = num_pages * page_bytes;

    int flags = MAP_PRIVATE | MAP_ANONYMOUS;
    int prot = PROT_READ | PROT_WRITE;

    if (use_huge) {
        flags |= MAP_HUGETLB;
        flags |= (page_bits << MAP_HUGE_SHIFT);
    }

    void* mem = mmap(nullptr, length, prot, flags, -1, 0);
    if (mem == MAP_FAILED) {
        return false;
    }

    data_ = static_cast<uint8_t*>(mem);
    allocated_size_ = length;
    return true;
}

void PackedArray::Set(size_t index, uint8_t value) {
    if (index >= n_) {
        throw std::out_of_range("Index out of range in PackedArray::Set");
    }
    if (value >= (1 << bits_per_element_)) {
        throw std::out_of_range(
            "Can't set value=" + std::to_string(value) + " into " + std::to_string(bits_per_element_) + " bits"
        );
    }

    size_t byte_index;
    size_t bit_offset;
    uint8_t mask;

    switch (bits_per_element_) {
        case 8:
            data_[index] = value & 0xFF;
            return;
        case 4:
            byte_index = index / 2;
            bit_offset = (index % 2) * 4;
            mask = 0x0F << bit_offset;
            value &= 0x0F;
            break;
        case 2:
            byte_index = index / 4;
            bit_offset = (index % 4) * 2;
            mask = 0x03 << bit_offset;
            value &= 0x03;
            break;
        default:
            throw std::logic_error("Invalid bits_per_element");
    }

    data_[byte_index] = (data_[byte_index] & ~mask) | (value << bit_offset);
}

uint8_t PackedArray::Get(size_t index) const {
    if (index >= n_) {
        throw std::out_of_range("Index out of range in PackedArray::Get");
    }

    size_t byte_index;
    size_t bit_offset;
    uint8_t mask;

    switch (bits_per_element_) {
        case 8:
            return data_[index];
        case 4:
            byte_index = index / 2;
            bit_offset = (index % 2) * 4;
            mask = 0x0F;
            break;
        case 2:
            byte_index = index / 4;
            bit_offset = (index % 4) * 2;
            mask = 0x03;
            break;
        default:
            throw std::logic_error("Invalid bits_per_element");
    }

    return (data_[byte_index] >> bit_offset) & mask;
}

void PackedArray::WriteToFile(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file for writing: " + filename);
    }
    file.write(reinterpret_cast<const char*>(data_), data_size_);
    if (!file) {
        throw std::runtime_error("Failed to write data to file: " + filename);
    }
}

void PackedArray::ReadFromFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file for reading: " + filename);
    }
    file.read(reinterpret_cast<char*>(data_), data_size_);
    if (!file) {
        throw std::runtime_error("Failed to read data from file: " + filename);
    }
}

}  // namespace utils
