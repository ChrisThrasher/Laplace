#pragma once

#include <cmath>
#include <ostream>
#include <type_traits>

namespace lp {

namespace detail {
    template <typename ValueType>
    constexpr auto pi = ValueType(3.1415926535897932385L);
}

template <typename ValueType>
class Angle {
public:
    static_assert(std::is_floating_point_v<ValueType>, "ValueType must be floating point");

    Angle() = default;

    [[nodiscard]] ValueType as_radians() const { return m_radians; }
    [[nodiscard]] ValueType as_degrees() const { return m_radians * 180 / detail::pi<ValueType>; }

private:
    explicit Angle(ValueType radians)
        : m_radians(radians)
    {
    }

    template <typename T>
    friend Angle<T> radians(T radians);

    template <typename T>
    friend Angle<T> degrees(T degrees);

    ValueType m_radians {};
};

template <typename ValueType>
[[nodiscard]] Angle<ValueType> radians(ValueType radians)
{
    return Angle(radians);
}

template <typename ValueType>
[[nodiscard]] Angle<ValueType> degrees(ValueType degrees)
{
    return Angle(degrees * detail::pi<ValueType> / 180);
}

template <typename ValueType>
auto& operator<<(std::ostream& stream, Angle<ValueType> angle)
{
    return stream << angle.as_radians() << " rad";
}

template <typename ValueType>
[[nodiscard]] bool operator==(Angle<ValueType> lhs, Angle<ValueType> rhs)
{
    return lhs.as_radians() == rhs.as_radians();
}

template <typename ValueType>
[[nodiscard]] Angle<ValueType> operator/(Angle<ValueType> lhs, ValueType divisor)
{
    return radians(lhs.as_radians() / divisor);
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
