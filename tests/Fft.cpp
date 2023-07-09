#include <Laplace/Fft.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("lp::fft")
{
    using namespace std::chrono_literals;

    SECTION("Minimum signal size")
    {
        const auto dft_data = lp::fft<double>({ { 0ms, 0. }, { 10ms, 0. } });
        REQUIRE(dft_data.size() == 2);
        CHECK(dft_data[0].amplitude == 0);
    }
}
