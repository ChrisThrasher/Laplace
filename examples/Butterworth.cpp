#include "ViewFilter.hpp"

#include <Laplace/Butterworth.hpp>

int main()
{
    auto filter = lp::Butterworth<double, 10>(lp::hertz(5.), 1.);
    view_filter("Butterworth", filter);
}
