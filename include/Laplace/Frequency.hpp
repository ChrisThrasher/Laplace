#pragma once

#include <Laplace/Angle.hpp>

#include <chrono>

namespace lp {

template <std::floating_point ValueType>
class Frequency {
public:
    constexpr Frequency() = default;

    [[nodiscard]] constexpr ValueType as_radians_per_second() const { return m_radians_per_second; }
    [[nodiscard]] constexpr ValueType as_hertz() const
    {
        return m_radians_per_second / (2 * std::numbers::pi_v<ValueType>);
    }

    [[nodiscard]] constexpr auto operator<=>(const Frequency&) const = default;

private:
    constexpr explicit Frequency(ValueType radians_per_second)
        : m_radians_per_second(radians_per_second)
    {
    }

    template <typename T>
    friend constexpr Frequency<T> radians_per_second(T radians_per_second);

    template <typename T>
    friend constexpr Frequency<T> hertz(T hertz);

    ValueType m_radians_per_second {}; // Frequency in radians per second
};

template <typename ValueType>
std::ostream& operator<<(std::ostream& stream, Frequency<ValueType> frequency)
{
    return stream << frequency.as_radians_per_second() << " rad/s";
}

template <typename ValueType>
[[nodiscard]] constexpr Frequency<ValueType> radians_per_second(ValueType radians_per_second)
{
    return Frequency(radians_per_second);
}

template <typename ValueType>
[[nodiscard]] constexpr Frequency<ValueType> hertz(ValueType hertz)
{
    return Frequency(hertz * 2 * std::numbers::pi_v<ValueType>);
}

template <typename ValueType>
[[nodiscard]] constexpr Frequency<ValueType> operator-(Frequency<ValueType> lhs, Frequency<ValueType> rhs)
{
    return radians_per_second(lhs.as_radians_per_second() - rhs.as_radians_per_second());
}

template <typename ValueType>
[[nodiscard]] constexpr Frequency<ValueType> operator-(Frequency<ValueType> frequency)
{
    return radians_per_second(-frequency.as_radians_per_second());
}

template <typename ValueType, typename Rep, typename Period>
[[nodiscard]] constexpr Angle<ValueType> operator*(Frequency<ValueType> frequency,
                                                   std::chrono::duration<Rep, Period> duration)
{
    return radians(frequency.as_radians_per_second()
                   * std::chrono::duration_cast<std::chrono::duration<ValueType>>(duration).count());
}

template <typename ValueType, typename Rep, typename Period>
[[nodiscard]] constexpr Angle<ValueType> operator*(std::chrono::duration<Rep, Period> duration,
                                                   Frequency<ValueType> frequency)
{
    return frequency * duration;
}

template <typename ValueType>
[[nodiscard]] constexpr Frequency<ValueType> operator*(ValueType factor, Frequency<ValueType> frequency)
{
    return radians_per_second(factor * frequency.as_radians_per_second());
}

template <typename ValueType, typename Rep, typename Period>
[[nodiscard]] constexpr Frequency<ValueType> operator/(Angle<ValueType> angle,
                                                       std::chrono::duration<Rep, Period> duration)
{
    return radians_per_second(angle.as_radians()
                              / std::chrono::duration_cast<std::chrono::duration<ValueType>>(duration).count());
}
}
