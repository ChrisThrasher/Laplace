#include <Laplace/Fft.hpp>

#include <catch2/catch_template_test_macros.hpp>

TEMPLATE_TEST_CASE("lp::fft", "", float, double, long double)
{
    using namespace std::chrono_literals;

    SECTION("Minimal signal of zeroes")
    {
        const auto dft_data = lp::fft<TestType>({ { 0ms, 0 }, { 10ms, 0 } });
        REQUIRE(dft_data.size() == 2);
        CHECK(dft_data[0].frequency == lp::radians_per_second<TestType>(0));
        CHECK(dft_data[0].phase == lp::radians<TestType>(0));
        CHECK(dft_data[0].amplitude == 0);
        CHECK(dft_data[1].frequency == lp::radians_per_second<TestType>(0));
        CHECK(dft_data[1].phase == lp::radians<TestType>(0));
        CHECK(dft_data[1].amplitude == 0);
    }

    SECTION("Non-zero DC gain")
    {
        const auto dft_data = lp::fft<TestType>({ { 0ms, 1 }, { 10ms, 1 }, { 20ms, 1 } });
        REQUIRE(dft_data.size() == 3);
        CHECK(dft_data[0].frequency == lp::radians_per_second<TestType>(0));
        CHECK(dft_data[0].phase == lp::radians<TestType>(0));
        CHECK(dft_data[0].amplitude == 1);
        CHECK(dft_data[1].frequency == lp::radians_per_second<TestType>(0));
        CHECK(dft_data[1].phase == lp::radians<TestType>(0));
        CHECK(dft_data[1].amplitude == 0);
        CHECK(dft_data[2].frequency == lp::radians_per_second<TestType>(0));
        CHECK(dft_data[2].phase == lp::radians<TestType>(0));
        CHECK(dft_data[2].amplitude == 0);
    }
}
