#include <Laplace/Rng.hpp>

#include <algorithm>
#include <array>

namespace lp {

std::mt19937& rng()
{
    thread_local auto generator = []() {
        auto seed_data = std::array<std::random_device::result_type, std::mt19937::state_size>();
        auto random_device = std::random_device();
        std::generate_n(seed_data.data(), seed_data.size(), std::ref(random_device));
        auto sequence = std::seed_seq(seed_data.begin(), seed_data.end());
        return std::mt19937(sequence);
    }();
    return generator;
}

}
