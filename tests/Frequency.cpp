#include <Laplace/Frequency.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

TEST_CASE("lp::Frequency")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_default_constructible_v<lp::Frequency<double>>);
        STATIC_CHECK(std::is_copy_constructible_v<lp::Frequency<double>>);
        STATIC_CHECK(std::is_copy_assignable_v<lp::Frequency<double>>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<lp::Frequency<double>>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<lp::Frequency<double>>);
    }

    SECTION("Construction")
    {
        SECTION("Default constructor")
        {
            const auto frequency = lp::Frequency<double>();
            CHECK(frequency.as_radians_per_second() == 0);
            CHECK(frequency.as_hertz() == 0);
        }

        SECTION("radians_per_second()")
        {
            const auto frequency = lp::radians_per_second(10.);
            CHECK(frequency.as_radians_per_second() == 10);
            CHECK_THAT(frequency.as_hertz(), Catch::Matchers::WithinRel(1.592, 1e-3));
        }

        SECTION("hertz()")
        {
            const auto frequency = lp::hertz(42.);
            CHECK_THAT(frequency.as_radians_per_second(), Catch::Matchers::WithinRel(263.9, 1e-3));
            CHECK(frequency.as_hertz() == 42);
        }
    }

    SECTION("Operators")
    {
        SECTION("operator-()")
        {
            CHECK(-lp::hertz(42.).as_hertz() == -42);
            CHECK(-lp::radians_per_second(-3.14).as_radians_per_second() == 3.14);
        }

        SECTION("operator==()")
        {
            CHECK(lp::hertz(0.) == lp::radians_per_second(0.));
            CHECK(lp::hertz(10.) == lp::hertz(10.));
            CHECK(lp::radians_per_second(10.) == lp::radians_per_second(10.));
        }

        SECTION("operator*(Frequency, std::chrono::duration)")
        {
            using namespace std::chrono_literals;
            CHECK(lp::radians_per_second(4.2) * 0s == lp::radians(0.));
            CHECK(lp::radians_per_second(4.2) * 1s == lp::radians(4.2));
            CHECK(lp::radians_per_second(4.2) * 10s == lp::radians(42.));
        }

        SECTION("operator*(std::chrono::duration, Frequency)")
        {
            using namespace std::chrono_literals;
            CHECK(0s * lp::radians_per_second(4.2) == lp::radians(0.));
            CHECK(1s * lp::radians_per_second(4.2) == lp::radians(4.2));
            CHECK(10s * lp::radians_per_second(4.2) == lp::radians(42.));
        }

        SECTION("operator*(ValueType, Frequency)")
        {
            CHECK(0. * lp::radians_per_second(10.) == lp::radians_per_second(0.));
            CHECK(2. * lp::radians_per_second(10.) == lp::radians_per_second(20.));
            CHECK(20. * lp::radians_per_second(10.) == lp::radians_per_second(200.));
        }
    }
}
