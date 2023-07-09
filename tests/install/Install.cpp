#include <Laplace/LowPass.hpp>

int main() { [[maybe_unused]] auto filter = lp::LowPass<double>(lp::hertz(10.), 0.0); }
