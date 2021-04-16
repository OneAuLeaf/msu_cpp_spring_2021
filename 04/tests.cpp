#include <iostream>
#include <cassert>

#include "BigInt.hpp"

void defaultUse()
{
    BigInt a = 200000000;
    std::cout << a << std::endl;
    BigInt b("20000000001");
    std::cout << b << std::endl;
    std::cout << a * b << std::endl;
}

int main()
{
    defaultUse();
}