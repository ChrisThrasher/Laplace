#include <Laplace/HighPass.hpp>

#include <catch2/catch_template_test_macros.hpp>

TEMPLATE_TEST_CASE("lp::HighPass", "", float, double, long double)
{
    SECTION("Type traits")
    {
        STATIC_CHECK(!std::is_default_constructible_v<lp::HighPass<TestType>>);
        STATIC_CHECK(std::is_copy_constructible_v<lp::HighPass<TestType>>);
        STATIC_CHECK(std::is_copy_assignable_v<lp::HighPass<TestType>>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<lp::HighPass<TestType>>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<lp::HighPass<TestType>>);
    }

    SECTION("Construction")
    {
        const auto filter = lp::HighPass(lp::hertz<TestType>(1), TestType(1.1));
        CHECK(filter.value() == TestType(1.1));
    }

    SECTION("operator()(ValueType)")
    {
        auto filter = lp::HighPass(lp::hertz<TestType>(5), TestType(0));
        CHECK(filter.value() == 0);
        for (std::size_t i = 0; i < 100; ++i) {
            CHECK(filter(1, std::chrono::milliseconds(100)) >= 0);
            CHECK(filter.value() >= 0);
            CHECK(filter.value() < 1);
        }
    }
}
