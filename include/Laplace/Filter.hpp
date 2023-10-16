#pragma once

#include <chrono>
#include <concepts>

namespace lp {

template <std::floating_point ValueType>
class Filter {
public:
    Filter() = default;
    virtual ~Filter() = default;

    virtual ValueType operator()(ValueType input, std::chrono::nanoseconds dt) = 0;
    [[nodiscard]] virtual ValueType value() const = 0;

protected:
    Filter(const Filter&) = default;
    Filter& operator=(const Filter&) = default;
    Filter(Filter&&) noexcept = default;
    Filter& operator=(Filter&&) noexcept = default;
};

}
