#pragma once

#include <Laplace/Frequency.hpp>

#include <cassert>
#include <complex>
#include <span>
#include <tuple>
#include <vector>

namespace lp {

template <std::floating_point ValueType>
struct DftDatum {
    lp::Frequency<ValueType> frequency;
    lp::Angle<ValueType> phase;
    ValueType amplitude {};

    [[nodiscard]] constexpr auto operator<=>(const DftDatum&) const = default;
};

template <typename ValueType>
std::ostream& operator<<(std::ostream& stream, DftDatum<ValueType> dft_datum)
{
    return stream << "{ " << dft_datum.frequency << ", " << dft_datum.phase << ", " << dft_datum.amplitude << " }";
}

template <std::floating_point ValueType>
[[nodiscard]] auto fft(const std::span<const ValueType> signal, const std::chrono::nanoseconds duration)
{
    assert(signal.size() >= 2);

    const auto size = ValueType(signal.size());
    assert(duration > std::chrono::nanoseconds(0));

    auto transformed = std::vector<DftDatum<ValueType>>(signal.size());
    for (std::size_t k = 0; k < signal.size() / 2; ++k) {
        const auto frequency = lp::hertz(ValueType(k)) / size;
        auto val = std::complex<ValueType>();
        for (std::size_t n = 0; n < signal.size(); ++n) {
            const auto phi = frequency * std::chrono::seconds(n);
            val += signal[n] * std::complex(cos(phi), -sin(phi));
        }
        val /= size;
        transformed[k] = { .frequency = lp::hertz(std::chrono::duration<ValueType>(k) / duration),
                           .phase = lp::radians(std::arg(val)),
                           .amplitude = std::abs(val) };
    }

    return transformed;
}

}
