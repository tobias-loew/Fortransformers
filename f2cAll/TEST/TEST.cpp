#include "stdafx.h"

#include "f2cpp.h"
#include "f2cpp_externals.h"
#include "TEST.h"

void foo(ft::fort_array_r<double, ft::bs<1, 10>> a, ft::fort_array_r<double, ft::bs<1, 10>> b, double & s)
{
    int i;
    ft::fort_array<int, ft::bs<1, 3>> j;

// assign-all
    a = 0.;
// assign-list
    j = {5, 2, 8};
// implied-do
    a = {PP_IMPLIED_DO(i + 1, i, 1, 10)};
// point-wise
    a = ft::cos(a) * .5;
// folding
    s = ft::sum(a);
// slicing
    b(ft::rng(3, 7)) = a(ft::rng(1, 5));
// strides
    b(ft::stride(ft::rng_first, ft::rng_last, 2)) = a(ft::stride(ft::rng_first, ft::rng_last, 2));
// indexing
    b(j) = a(j) + 100;
} /* foo */

