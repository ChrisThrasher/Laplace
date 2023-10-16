#pragma once

#include <Laplace/Filter.hpp>
#include <Laplace/Frequency.hpp>

#include <cassert>
#include <cmath>

namespace lp {

template <std::floating_point ValueType>
class LowPass final : public Filter<ValueType> {
public:
    LowPass(Frequency<ValueType> cutoff_frequency, ValueType initial_value)
        : m_cutoff_frequency(cutoff_frequency)
        , m_output(initial_value)
    {
    }

    ValueType operator()(ValueType input, std::chrono::nanoseconds dt) override
    {
        assert(std::isfinite(input));

        // out[n] = a * in[n] + (1 - a) * out[n-1]
        const auto coefficient = exp(-m_cutoff_frequency * dt);
        return m_output = coefficient * m_output + (1 - coefficient) * input;
    }

    [[nodiscard]] ValueType value() const override { return m_output; }

private:
    Frequency<ValueType> m_cutoff_frequency;
    ValueType m_output;
};

}
