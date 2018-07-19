#pragma once

#include "f2cpp.h"

#include "f2cpp_typedefs.h"

void foo(ft::fort_array_r<double, ft::bs<1, 10>> a, ft::fort_array_r<double, ft::bs<1, 10>> b, double & s);
