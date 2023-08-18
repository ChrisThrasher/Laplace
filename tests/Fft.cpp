#include <Laplace/Fft.hpp>

#include <catch2/catch_template_test_macros.hpp>

TEMPLATE_TEST_CASE("lp::fft", "", float, double, long double)
{
    using namespace std::chrono_literals;

    SECTION("Minimum signal size")
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
}
