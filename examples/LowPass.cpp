#include "ViewFilter.hpp"

#include <Laplace/LowPass.hpp>

int main()
{
    auto filter = lp::LowPass(lp::hertz(3.), 0.);
    view_filter("Low Pass", filter);
}
