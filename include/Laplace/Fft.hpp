#pragma once

#include <Laplace/Frequency.hpp>

#include <cassert>
#include <chrono>
#include <complex>
#include <vector>

namespace lp {

template <typename ValueType>
struct DftDatum {
    static_assert(std::is_floating_point_v<ValueType>, "ValueType must be floating point");

    lp::Frequency<ValueType> frequency;
    lp::Angle<ValueType> phase;
    ValueType amplitude {};
};

template <typename ValueType>
[[nodiscard]] bool operator==(const DftDatum<ValueType>& lhs, const DftDatum<ValueType>& rhs)
{
    return std::tie(lhs.frequency, lhs.phase, lhs.amplitude) == std::tie(rhs.frequency, rhs.phase, rhs.amplitude);
}

template <typename ValueType>
auto fft(const std::vector<ValueType>& signal, const std::chrono::nanoseconds duration)
{
    assert(signal.size() >= 2);

    const auto size = ValueType(signal.size());
    assert(duration > std::chrono::nanoseconds(0));

    auto transformed = std::vector<DftDatum<ValueType>>(signal.size());
    for (std::size_t k = 0; k < signal.size() / 2; ++k) {
        const auto frequency = lp::hertz(ValueType(k));
        auto val = std::complex<ValueType>();
        for (std::size_t n = 0; n < signal.size(); ++n) {
            const auto phi = (frequency * std::chrono::seconds(n)) / size;
            val += signal[n] * std::complex(cos(phi), -sin(phi));
        }
        val /= size;
        transformed[k]
            = { lp::hertz(std::chrono::duration<ValueType>(k) / duration), lp::radians(std::arg(val)), std::abs(val) };
    }

    return transformed;
}

}
