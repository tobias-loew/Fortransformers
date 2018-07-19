
#include <iostream>
#include "f2cpp.h"

void foo(ft::fort_array_r<double, ft::bs<1, 10>> a, ft::fort_array_r<double, ft::bs<1, 10>> b, double & s);
void bar(ft::fort_array_r<double, ft::bs<1, 10>> a, ft::fort_array_r<double, ft::bs<1, 10>> b, double & s);


int main() {
    {
        ft::fort_array<double, ft::bs<1, 10>> a;
        ft::fort_array<double, ft::bs<1, 10>> b;
        double s;

        foo(a, b, s);

        std::cout << "call to foo [f77]" << std::endl;
        std::cout << "a:" << std::endl;
        for (auto v : a) {
            std::cout << v << ", ";
        }
        std::cout << std::endl;
        std::cout << "b:" << std::endl;
        for (auto v : b) {
            std::cout << v << ", ";
        }
        std::cout << std::endl;
        std::cout << "s:" << std::endl;
        std::cout << s << std::endl;
        std::cout << std::endl;
    }
     
    {
        ft::fort_array<double, ft::bs<1, 10>> a;
        ft::fort_array<double, ft::bs<1, 10>> b;
        double s;

        bar(a, b, s);

        std::cout << "call to bar [f90]" << std::endl;
        std::cout << "a:" << std::endl;
        for (auto v : a) {
            std::cout << v << ", ";
        }
        std::cout << std::endl;
        std::cout << "b:" << std::endl;
        for (auto v : b) {
            std::cout << v << ", ";
        }
        std::cout << std::endl;
        std::cout << "s:" << std::endl;
        std::cout << s << std::endl;
        std::cout << std::endl;
    }

    return 0;
}