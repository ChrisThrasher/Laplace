#include <Laplace/Filter.hpp>

#include <catch2/catch_template_test_macros.hpp>

TEMPLATE_TEST_CASE("lp::Filter", "", float, double, long double)
{
    SECTION("Type traits")
    {
        // STATIC_CHECK(!std::is_copy_constructible_v<lp::Filter<double>>); MinGW thinks this type trait is true
        STATIC_CHECK(std::is_copy_assignable_v<lp::Filter<double>>);
        STATIC_CHECK(!std::is_nothrow_move_constructible_v<lp::Filter<double>>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<lp::Filter<double>>);
    }
}
