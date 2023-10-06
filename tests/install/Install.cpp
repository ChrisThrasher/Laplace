#include <Laplace/Angle.hpp>
#include <Laplace/Fft.hpp>
#include <Laplace/Filter.hpp>
#include <Laplace/Frequency.hpp>
#include <Laplace/HighPass.hpp>
#include <Laplace/LowPass.hpp>
#include <Laplace/Rng.hpp>

int main()
{
    [[maybe_unused]] auto rng = lp::rng();
    [[maybe_unused]] auto lpf = lp::LowPass(lp::hertz(10.), 0.);
    [[maybe_unused]] auto hpf = lp::HighPass(lp::hertz(10.), 0.);
}
