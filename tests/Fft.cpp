#include <Laplace/Fft.hpp>

#include <catch2/catch_template_test_macros.hpp>

TEMPLATE_TEST_CASE("lp::DftDatum", "", float, double, long double)
{
    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_aggregate_v<lp::DftDatum<TestType>>);
        STATIC_CHECK(std::is_default_constructible_v<lp::DftDatum<TestType>>);
        STATIC_CHECK(std::is_copy_constructible_v<lp::DftDatum<TestType>>);
        STATIC_CHECK(std::is_copy_assignable_v<lp::DftDatum<TestType>>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<lp::DftDatum<TestType>>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<lp::DftDatum<TestType>>);
    }

    SECTION("Construction")
    {
        const auto dft_datum = lp::DftDatum<TestType>();
        CHECK(dft_datum.frequency == lp::radians_per_second<TestType>(0));
        CHECK(dft_datum.phase == lp::degrees<TestType>(0));
        CHECK(dft_datum.amplitude == 0);
    }

    SECTION("Operators")
    {
        SECTION("operator<<()")
        {
            auto stream = std::ostringstream();
            stream << lp::DftDatum<TestType> { lp::radians_per_second<TestType>(1), lp::radians<TestType>(2), 3 };
            CHECK(stream.str() == "{ 1 rad/s, 2 rad, 3 }");
        }

        SECTION("operator==()")
        {
            CHECK(lp::DftDatum<TestType>() == lp::DftDatum<TestType>());
            CHECK(lp::DftDatum<TestType> { lp::hertz<TestType>(1), lp::degrees<TestType>(2), 3 }
                  == lp::DftDatum<TestType> { lp::hertz<TestType>(1), lp::degrees<TestType>(2), 3 });
        }
    }
}

TEMPLATE_TEST_CASE("lp::fft", "", float, double, long double)
{
    using namespace std::chrono_literals;

    SECTION("Minimal signal of zeroes")
    {
        CHECK(lp::fft<TestType>({ 0, 0 }, 10ms)
              == std::vector<lp::DftDatum<TestType>> { { lp::hertz<TestType>(0), lp::degrees<TestType>(0), 0 },
                                                       { lp::hertz<TestType>(0), lp::degrees<TestType>(0), 0 } });
    }

    SECTION("Minimal signal of zeroes")
    {
        CHECK(lp::fft<TestType>({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 1s)
              == std::vector<lp::DftDatum<TestType>> { { lp::hertz<TestType>(0), lp::degrees<TestType>(0), 0 },
                                                       { lp::hertz<TestType>(1), lp::degrees<TestType>(0), 0 },
                                                       { lp::hertz<TestType>(2), lp::degrees<TestType>(0), 0 },
                                                       { lp::hertz<TestType>(3), lp::degrees<TestType>(0), 0 },
                                                       { lp::hertz<TestType>(4), lp::degrees<TestType>(0), 0 },
                                                       { lp::hertz<TestType>(0), lp::degrees<TestType>(0), 0 },
                                                       { lp::hertz<TestType>(0), lp::degrees<TestType>(0), 0 },
                                                       { lp::hertz<TestType>(0), lp::degrees<TestType>(0), 0 },
                                                       { lp::hertz<TestType>(0), lp::degrees<TestType>(0), 0 },
                                                       { lp::hertz<TestType>(0), lp::degrees<TestType>(0), 0 } });
    }

    SECTION("Non-zero DC gain")
    {
        CHECK(lp::fft<TestType>({ 1, 1, 1 }, 20ms)
              == std::vector<lp::DftDatum<TestType>> { { lp::hertz<TestType>(0), lp::degrees<TestType>(0), 1 },
                                                       { lp::hertz<TestType>(0), lp::degrees<TestType>(0), 0 },
                                                       { lp::hertz<TestType>(0), lp::degrees<TestType>(0), 0 } });
    }
}
