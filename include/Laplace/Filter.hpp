#pragma once

#include <chrono>
#include <type_traits>

namespace lp {

template <typename ValueType>
class Filter {
public:
    static_assert(std::is_floating_point_v<ValueType>, "ValueType must be floating point");

    virtual ~Filter() = default;

    virtual ValueType operator()(ValueType input, std::chrono::nanoseconds dt) noexcept = 0;
    [[nodiscard]] virtual ValueType value() const noexcept = 0;
};

}
