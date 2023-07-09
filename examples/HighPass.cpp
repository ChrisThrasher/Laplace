#include "ViewFilter.hpp"

#include <Laplace/HighPass.hpp>

int main()
{
    auto filter = lp::HighPass(lp::hertz(5.), 1.);
    view_filter("High Pass", filter);
}
