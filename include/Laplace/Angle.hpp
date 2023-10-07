#pragma once

#include <cmath>
#include <concepts>
#include <ostream>

namespace lp {

namespace detail {
    template <std::floating_point ValueType>
    constexpr auto pi = ValueType(3.1415926535897932385L);
}

template <std::floating_point ValueType>
class Angle {
public:
    constexpr Angle() = default;

    [[nodiscard]] constexpr ValueType as_radians() const { return m_radians; }
    [[nodiscard]] constexpr ValueType as_degrees() const { return m_radians * 180 / detail::pi<ValueType>; }

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
    return Angle(degrees * detail::pi<ValueType> / 180);
}

template <typename ValueType>
[[nodiscard]] constexpr Angle<ValueType> operator/(Angle<ValueType> lhs, ValueType divisor)
{
    return radians(lhs.as_radians() / divisor);
}

template <typename ValueType>
[[nodiscard]] constexpr Angle<ValueType> operator*(Angle<ValueType> lhs, ValueType factor)
{
    return radians(lhs.as_radians() * factor);
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
