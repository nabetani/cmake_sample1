#include "sum_of_digits.h"
#include <stdexcept>

int sum_of_digits(int a, int b) {
  if (a < 0 || b < 2) {
    throw std::runtime_error("invalid param in sum_of_digits");
  }
  int sum = 0;
  while (a != 0) {
    sum += a % b;
    a /= b;
  }
  return sum;
}
