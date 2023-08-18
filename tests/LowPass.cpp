#include <Laplace/LowPass.hpp>

#include <catch2/catch_template_test_macros.hpp>

TEMPLATE_TEST_CASE("lp::LowPass", "", float, double, long double)
{
    SECTION("Type traits")
    {
        STATIC_CHECK(!std::is_default_constructible_v<lp::LowPass<TestType>>);
        STATIC_CHECK(std::is_copy_constructible_v<lp::LowPass<TestType>>);
        STATIC_CHECK(std::is_copy_assignable_v<lp::LowPass<TestType>>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<lp::LowPass<TestType>>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<lp::LowPass<TestType>>);
    }

    SECTION("Construction")
    {
        const auto filter = lp::LowPass(lp::hertz<TestType>(1), TestType(1.1));
        CHECK(filter.value() == TestType(1.1));
    }

    SECTION("operator()(ValueType)")
    {
        auto filter = lp::LowPass(lp::radians_per_second<TestType>(1), TestType(0));
        CHECK(filter.value() == 0);
        for (std::size_t i = 0; i < 100; ++i) {
            const auto previous = filter.value();
            CHECK(filter(1, std::chrono::milliseconds(100)) > 0);
            CHECK(filter.value() > 0);
            CHECK(filter.value() < 1);
            CHECK(filter.value() > previous);
        }
    }
}
