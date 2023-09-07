#include <Laplace/Rng.hpp>

#include <catch2/catch_test_macros.hpp>

#include <thread>

TEST_CASE("lp::rng")
{
    const auto* rng = &lp::rng();

    SECTION("Called in same thread")
    {
        CHECK(rng == &lp::rng());
        CHECK(rng == &lp::rng());
    }

    SECTION("Called from separate threads")
    {
        auto thread1 = std::thread([rng] { CHECK(rng != &lp::rng()); });
        auto thread2 = std::thread([rng] { CHECK(rng != &lp::rng()); });
        thread1.join();
        thread2.join();
    }
}
