#pragma once

#include <chrono>
#include <type_traits>

namespace lp {

template <typename ValueType>
class Filter {
public:
    static_assert(std::is_floating_point_v<ValueType>, "ValueType must be floating point");

    Filter() = default;
    virtual ~Filter() = default;
    Filter(const Filter&) = default;
    Filter& operator=(const Filter&) = default;
    Filter(Filter&&) noexcept = default;
    Filter& operator=(Filter&&) noexcept = default;

    virtual ValueType operator()(ValueType input, std::chrono::nanoseconds dt) = 0;
    [[nodiscard]] virtual ValueType value() const = 0;
};

}
