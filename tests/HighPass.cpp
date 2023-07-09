#include <Laplace/HighPass.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("lp::HighPass")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(!std::is_default_constructible_v<lp::HighPass<double>>);
        STATIC_CHECK(std::is_copy_constructible_v<lp::HighPass<double>>);
        STATIC_CHECK(std::is_copy_assignable_v<lp::HighPass<double>>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<lp::HighPass<double>>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<lp::HighPass<double>>);
    }

    SECTION("Construction")
    {
        const auto filter = lp::HighPass(lp::hertz(1.), 1.1);
        CHECK(filter.value() == 1.1);
    }

    SECTION("operator()(ValueType)")
    {
        auto filter = lp::HighPass(lp::hertz(5.), 0.);
        CHECK(filter.value() == 0);
        for (std::size_t i = 0; i < 100; ++i) {
            CHECK(filter(1, std::chrono::milliseconds(100)) >= 0);
            CHECK(filter.value() >= 0);
            CHECK(filter.value() < 1);
        }
    }
}
