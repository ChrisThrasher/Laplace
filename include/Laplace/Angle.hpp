#pragma once

#include <cassert>
#include <cmath>
#include <concepts>
#include <numbers>
#include <ostream>

namespace lp {

template <std::floating_point ValueType>
class Angle {
public:
    constexpr Angle() = default;

    [[nodiscard]] constexpr ValueType as_radians() const { return m_radians; }
    [[nodiscard]] constexpr ValueType as_degrees() const { return m_radians * 180 / std::numbers::pi_v<ValueType>; }

    [[nodiscard]] constexpr auto operator<=>(const Angle&) const = default;

private:
    constexpr explicit Angle(ValueType radians)
        : m_radians(radians)
    {
    }

    template <typename T>
    friend constexpr Angle<T> radians(T radians);

    template <typename T>
    friend constexpr Angle<T> degrees(T degrees);

    ValueType m_radians {};
};

template <typename ValueType>
std::ostream& operator<<(std::ostream& stream, Angle<ValueType> angle)
{
    return stream << angle.as_radians() << " rad";
}

template <typename ValueType>
[[nodiscard]] constexpr Angle<ValueType> radians(ValueType radians)
{
    return Angle(radians);
}

template <typename ValueType>
[[nodiscard]] constexpr Angle<ValueType> degrees(ValueType degrees)
{
    return Angle(degrees * std::numbers::pi_v<ValueType> / 180);
}

template <typename ValueType>
[[nodiscard]] constexpr Angle<ValueType> operator/(Angle<ValueType> angle, ValueType divisor)
{
    assert(divisor != 0 && "lp::Angle::operator/: Cannot divide by zero");
    return radians(angle.as_radians() / divisor);
}

template <typename ValueType>
[[nodiscard]] constexpr Angle<ValueType> operator*(Angle<ValueType> angle, ValueType factor)
{
    return radians(angle.as_radians() * factor);
}

template <typename ValueType>
[[nodiscard]] ValueType exp(Angle<ValueType> angle)
{
    return std::exp(angle.as_radians());
}

template <typename ValueType>
[[nodiscard]] ValueType sin(Angle<ValueType> angle)
{
    return std::sin(angle.as_radians());
}

template <typename ValueType>
[[nodiscard]] ValueType cos(Angle<ValueType> angle)
{
    return std::cos(angle.as_radians());
}

}
