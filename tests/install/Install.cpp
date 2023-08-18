#include <Laplace/LowPass.hpp>

int main() { [[maybe_unused]] auto filter = lp::LowPass(lp::hertz(10.), 0.); }
