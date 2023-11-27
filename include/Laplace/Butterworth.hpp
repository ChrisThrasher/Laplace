#pragma once

#include <Laplace/Filter.hpp>
#include <Laplace/Frequency.hpp>
#include <array>

#include <cassert>
#include <cmath>

namespace lp {



template <std::floating_point ValueType, std::size_t order>
class Butterworth final : public Filter<ValueType> {
public:
    Butterworth(Frequency<ValueType> cutoff_frequency, ValueType initial_value)
        : m_cutoff_frequency(cutoff_frequency)
        , m_output(initial_value)
    {
      assert(order > 1);

      // calculate butterworth coefficients recursively
      m_coefficients[0] = 1; // a0 = 1
      auto const gamma = std::numbers::pi_v<ValueType> / (static_cast<ValueType>(2) * static_cast<ValueType>(order));
      for (std::size_t k = 1; k <= order / 2; ++k) {
        m_coefficients[k] = m_coefficients[k-1] * std::cos(static_cast<ValueType>(k - 1) * gamma) / std::sin(static_cast<ValueType>(k) * gamma);
      }
      // ak = a(n-k)
      for (std::size_t k = order / 2; k < m_coefficients.size(); ++k) {
        m_coefficients[k] = m_coefficients[order - k];
      }
    }

    ValueType operator()(ValueType input, std::chrono::nanoseconds /*unused*/) override
    {
        assert(std::isfinite(input));
        m_output = input;

        // out[n] = in[n] + 2 * in[n−1] + in[n−2] − a1 * y[n−1]− a2 * y[n−2]
        return m_output;
    }

    [[nodiscard]] ValueType value() const override { return m_output; }

private:
    Frequency<ValueType> m_cutoff_frequency;
    std::array<ValueType, order + 1> m_coefficients;
    ValueType m_output;
    ValueType m_input {};
};

}
