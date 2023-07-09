#pragma once

#include <Laplace/Frequency.hpp>

#include <cassert>
#include <chrono>
#include <complex>
#include <vector>

namespace lp {

template <typename ValueType>
struct Datum {
    static_assert(std::is_floating_point_v<ValueType>, "ValueType must be floating point");

    std::chrono::duration<ValueType> time {};
    ValueType amplitude {};
};

template <typename ValueType>
struct DftDatum {
    static_assert(std::is_floating_point_v<ValueType>, "ValueType must be floating point");

    lp::Frequency<ValueType> frequency;
    lp::Angle<ValueType> phase;
    ValueType amplitude {};
};

template <typename ValueType>
using Signal = std::vector<Datum<ValueType>>;

template <typename ValueType>
auto fft(const Signal<ValueType>& signal)
{
    assert(signal.size() >= 2);
    const auto duration = signal.back().time - signal.front().time;
    const auto size = ValueType(signal.size());

    auto transformed = std::vector<DftDatum<ValueType>>(signal.size());
    for (std::size_t k = 0; k < signal.size() / 2; ++k) {
        const auto frequency = lp::hertz(ValueType(k));
        auto val = std::complex<ValueType>();
        for (std::size_t n = 0; n < signal.size(); ++n) {
            const auto phi = (frequency * std::chrono::seconds(n)) / size;
            val += signal[n].amplitude * std::complex(cos(phi), -sin(phi));
        }
        val /= size;
        transformed[k]
            = { lp::hertz(std::chrono::duration<ValueType>(k) / duration), lp::radians(std::arg(val)), std::abs(val) };
    }

    return transformed;
}

}
