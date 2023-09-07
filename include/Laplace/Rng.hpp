#pragma once

#include <Laplace/Export.hpp>

#include <random>

namespace lp {

[[nodiscard]] LAPLACE_EXPORT std::mt19937& rng();

}
