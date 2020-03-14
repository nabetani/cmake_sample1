#include "sum_of_digits.h"
#include <exception>

int sum_of_digits(int a, int b) {
  int sum = 0;
  while (a != 0) {
    sum += a % b;
    a /= b;
  }
  return sum;
}
