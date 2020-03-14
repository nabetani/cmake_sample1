#include <cstdlib>
#include <iostream>

#if !defined TESTBUILD

#include "sum_of_digits.h"

int main(int argc, char const *argv[]) {
  int a = argc < 2 ? 1234567 : std::atoi(argv[1]);
  int b = argc < 3 ? 10 : std::atoi(argv[2]);

  std::cout << "num=" << a << "\n"
            << "base=" << b << "\n"
            << "sum of each digits: " << sum_of_digits(a, b) << std::endl;
#if defined DEBUG
  std::cout << "DEBUG!" << std::endl;
#endif
#if defined _DEBUG
  std::cout << "_DEBUG!" << std::endl;
#endif
#if defined NDEBUG
  std::cout << "NDEBUG!" << std::endl;
#endif
}

#endif
