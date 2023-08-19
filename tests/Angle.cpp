#include <Laplace/Angle.hpp>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <sstream>

TEMPLATE_TEST_CASE("lp::Angle", "", float, double, long double)
{
    SECTION("Type traits")
    {
        STATIC_CHECK(sizeof(lp::Angle<TestType>) == sizeof(TestType));
        STATIC_CHECK(std::is_default_constructible_v<lp::Angle<TestType>>);
        STATIC_CHECK(std::is_copy_constructible_v<lp::Angle<TestType>>);
        STATIC_CHECK(std::is_copy_assignable_v<lp::Angle<TestType>>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<lp::Angle<TestType>>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<lp::Angle<TestType>>);
    }

    SECTION("Construction")
    {
        SECTION("Default constructor")
        {
            constexpr auto angle = lp::Angle<TestType>();
            STATIC_CHECK(angle.as_radians() == 0);
            STATIC_CHECK(angle.as_degrees() == 0);
        }

        SECTION("radians()")
        {
            constexpr auto angle = lp::radians<TestType>(1);
            STATIC_CHECK(angle.as_radians() == 1);
            CHECK_THAT(angle.as_degrees(), Catch::Matchers::WithinRel(57.2957795131, 1e-3));
        }

        SECTION("degrees()")
        {
            constexpr auto angle = lp::degrees<TestType>(10);
            CHECK_THAT(angle.as_radians(), Catch::Matchers::WithinRel(0.1745329252, 1e-3));
            CHECK_THAT(angle.as_degrees(), Catch::Matchers::WithinAbs(10., 1e-6));
        }
    }

    SECTION("Operators")
    {
        SECTION("operator<<()")
        {
            auto stream = std::ostringstream();
            stream << lp::radians<TestType>(3.14f);
            CHECK(stream.str() == "3.14 rad");
        }

        SECTION("operator==()")
        {
            STATIC_CHECK(lp::degrees<TestType>(0) == lp::radians<TestType>(0));
            STATIC_CHECK(lp::degrees<TestType>(10) == lp::degrees<TestType>(10));
            STATIC_CHECK(lp::radians<TestType>(3.14f) == lp::radians<TestType>(3.14f));
        }

        SECTION("operator/()")
        {
            STATIC_CHECK(lp::radians<TestType>(4) / TestType(4) == lp::radians<TestType>(1));
            STATIC_CHECK(lp::radians<TestType>(0) / TestType(10) == lp::radians<TestType>(0));
        }
    }

    SECTION("Trigonometry")
    {
        SECTION("exp()")
        {
            CHECK(lp::exp(lp::radians<TestType>(1)) == std::exp(TestType(1)));
            CHECK(lp::exp(lp::degrees<TestType>(100)) == std::exp(lp::degrees<TestType>(100).as_radians()));
        }

        SECTION("sin()")
        {
            CHECK(lp::sin(lp::degrees<TestType>(0)) == 0);
            CHECK(lp::sin(lp::radians<TestType>(1.5)) == std::sin(TestType(1.5)));
            CHECK(lp::sin(lp::degrees<TestType>(15)) == std::sin(lp::degrees<TestType>(15).as_radians()));
        }

        SECTION("cos()")
        {
            CHECK(lp::cos(lp::degrees<TestType>(0)) == 1);
            CHECK(lp::cos(lp::radians<TestType>(1.5)) == std::cos(TestType(1.5)));
            CHECK(lp::cos(lp::degrees<TestType>(15)) == std::cos(lp::degrees<TestType>(15).as_radians()));
        }
    }
}
