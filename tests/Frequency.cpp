#include <Laplace/Frequency.hpp>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <sstream>

TEMPLATE_TEST_CASE("lp::Frequency", "", float, double, long double)
{
    SECTION("Type traits")
    {
        STATIC_CHECK(sizeof(lp::Frequency<TestType>) == sizeof(TestType));
        STATIC_CHECK(std::is_default_constructible_v<lp::Frequency<TestType>>);
        STATIC_CHECK(std::is_copy_constructible_v<lp::Frequency<TestType>>);
        STATIC_CHECK(std::is_copy_assignable_v<lp::Frequency<TestType>>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<lp::Frequency<TestType>>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<lp::Frequency<TestType>>);
    }

    SECTION("Construction")
    {
        SECTION("Default constructor")
        {
            constexpr auto frequency = lp::Frequency<TestType>();
            STATIC_CHECK(frequency.as_radians_per_second() == 0);
            STATIC_CHECK(frequency.as_hertz() == 0);
        }

        SECTION("radians_per_second()")
        {
            constexpr auto frequency = lp::radians_per_second<TestType>(10);
            STATIC_CHECK(frequency.as_radians_per_second() == 10);
            CHECK_THAT(frequency.as_hertz(), Catch::Matchers::WithinRel(1.592, 1e-3));
        }

        SECTION("hertz()")
        {
            constexpr auto frequency = lp::hertz<TestType>(42);
            CHECK_THAT(frequency.as_radians_per_second(), Catch::Matchers::WithinRel(263.9, 1e-3));
            STATIC_CHECK(frequency.as_hertz() == 42);
        }
    }

    SECTION("Operators")
    {
        using namespace std::chrono_literals;

        SECTION("operator<=>()")
        {
            STATIC_CHECK(lp::hertz<TestType>(0) == lp::radians_per_second<TestType>(0));
            STATIC_CHECK(lp::hertz<TestType>(10) < lp::hertz<TestType>(11));
            STATIC_CHECK(lp::radians_per_second<TestType>(10) > lp::radians_per_second<TestType>(9));
        }

        SECTION("operator<<()")
        {
            auto stream = std::ostringstream();
            stream << lp::radians_per_second<TestType>(3.14f);
            CHECK(stream.str() == "3.14 rad/s");
        }

        SECTION("operator-(Frequency, Frequency)")
        {
            STATIC_CHECK(lp::Frequency<TestType>() - lp::Frequency<TestType>() == lp::Frequency<TestType>());
            STATIC_CHECK(lp::radians_per_second<TestType>(10) - lp::radians_per_second<TestType>(5)
                         == lp::radians_per_second<TestType>(5));
        }

        SECTION("operator-(Frequency)")
        {
            STATIC_CHECK(-lp::hertz<TestType>(42).as_hertz() == -42);
            STATIC_CHECK(-lp::radians_per_second<TestType>(-3.14f).as_radians_per_second() == TestType(3.14f));
        }

        SECTION("operator*(Frequency, std::chrono::duration)")
        {
            STATIC_CHECK(lp::radians_per_second<TestType>(4) * 0s == lp::radians<TestType>(0));
            STATIC_CHECK(lp::radians_per_second<TestType>(4) * 1s == lp::radians<TestType>(4));
            STATIC_CHECK(lp::radians_per_second<TestType>(4) * 10s == lp::radians<TestType>(40));
        }

        SECTION("operator*(std::chrono::duration, Frequency)")
        {
            STATIC_CHECK(0s * lp::radians_per_second<TestType>(4) == lp::radians<TestType>(0));
            STATIC_CHECK(1s * lp::radians_per_second<TestType>(4) == lp::radians<TestType>(4));
            STATIC_CHECK(10s * lp::radians_per_second<TestType>(4) == lp::radians<TestType>(40));
        }

        SECTION("operator*(ValueType, Frequency)")
        {
            STATIC_CHECK(TestType(0) * lp::radians_per_second<TestType>(10) == lp::radians_per_second<TestType>(0));
            STATIC_CHECK(TestType(2) * lp::radians_per_second<TestType>(10) == lp::radians_per_second<TestType>(20));
            STATIC_CHECK(TestType(20) * lp::radians_per_second<TestType>(10) == lp::radians_per_second<TestType>(200));
        }

        SECTION("operator/(Angle, std::chrono::duration)")
        {
            STATIC_CHECK(lp::degrees<TestType>(360) / 1s == lp::hertz<TestType>(1));
            STATIC_CHECK(lp::radians<TestType>(5) / 1s == lp::radians_per_second<TestType>(5));
            STATIC_CHECK(lp::radians<TestType>(3600) / 1min == lp::radians_per_second<TestType>(60));
        }

        SECTION("operator/(Frequency, ValueType)")
        {
            STATIC_CHECK(lp::radians_per_second<TestType>(0) / TestType(10) == lp::radians_per_second<TestType>(0));
            STATIC_CHECK(lp::radians_per_second<TestType>(10) / TestType(10) == lp::radians_per_second<TestType>(1));
        }
    }
}
