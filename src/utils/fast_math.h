#pragma once

#include <stddef.h>
#include <array>

namespace utils {

constexpr size_t factorial(size_t n) { return n <= 1 ? 1 : n * factorial(n - 1); }

template <size_t... I>
constexpr std::array<size_t, sizeof...(I)> make_factorial_array(std::index_sequence<I...>) {
    return {{factorial(I)...}};
}

constexpr auto FACTORIAL = make_factorial_array(std::make_index_sequence<13>{});

constexpr size_t power_of_two(size_t exponent) { return 1LL << exponent; }

template <size_t... I>
constexpr std::array<size_t, sizeof...(I)> make_power_of_two_array(std::index_sequence<I...>) {
    return {{power_of_two(I)...}};
}

constexpr auto POWERS_OF_TWO = make_power_of_two_array(std::make_index_sequence<13>{});

constexpr size_t power_of_three(size_t exponent) { return exponent == 0 ? 1 : 3 * power_of_three(exponent - 1); }

template <size_t... I>
constexpr std::array<size_t, sizeof...(I)> make_power_of_three_array(std::index_sequence<I...>) {
    return {{power_of_three(I)...}};
}

constexpr auto POWERS_OF_THREE = make_power_of_three_array(std::make_index_sequence<13>{});

}  // namespace utils
