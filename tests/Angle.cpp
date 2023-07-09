#include <Laplace/Angle.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

TEST_CASE("lp::Angle")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_default_constructible_v<lp::Angle<double>>);
        STATIC_CHECK(std::is_copy_constructible_v<lp::Angle<double>>);
        STATIC_CHECK(std::is_copy_assignable_v<lp::Angle<double>>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<lp::Angle<double>>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<lp::Angle<double>>);
    }

    SECTION("Construction")
    {
        SECTION("Default constructor")
        {
            const auto angle = lp::Angle<double>();
            CHECK(angle.as_radians() == 0);
            CHECK(angle.as_degrees() == 0);
        }

        SECTION("radians()")
        {
            const auto angle = lp::radians(1.);
            CHECK(angle.as_radians() == 1);
            CHECK_THAT(angle.as_degrees(), Catch::Matchers::WithinRel(57.2957795131, 1e-3));
        }

        SECTION("degrees()")
        {
            const auto angle = lp::degrees(10.);
            CHECK_THAT(angle.as_radians(), Catch::Matchers::WithinRel(0.1745329252, 1e-3));
            CHECK(angle.as_degrees() == 10);
        }
    }

    SECTION("Operators")
    {
        SECTION("operator==()")
        {
            CHECK(lp::degrees(0.) == lp::radians(0.));
            CHECK(lp::degrees(10.) == lp::degrees(10.));
            CHECK(lp::radians(3.14) == lp::radians(3.14));
        }

        SECTION("operator/()")
        {
            CHECK(lp::radians(4.) / 4. == lp::radians(1.));
            CHECK(lp::radians(0.) / 10. == lp::radians(0.));
        }
    }

    SECTION("Trigonometry")
    {
        SECTION("exp()")
        {
            CHECK(lp::exp(lp::radians(1.)) == std::exp(1.));
            CHECK(lp::exp(lp::degrees(100.)) == std::exp(lp::degrees(100.).as_radians()));
        }

        SECTION("sin()")
        {
            CHECK(lp::sin(lp::degrees(0.)) == 0);
            CHECK(lp::sin(lp::radians(1.5)) == std::sin(1.5));
            CHECK(lp::sin(lp::degrees(15.)) == std::sin(lp::degrees(15.).as_radians()));
        }

        SECTION("cos()")
        {
            CHECK(lp::cos(lp::degrees(0.)) == 1);
            CHECK(lp::cos(lp::radians(1.5)) == std::cos(1.5));
            CHECK(lp::cos(lp::degrees(15.)) == std::cos(lp::degrees(15.).as_radians()));
        }
    }
}
