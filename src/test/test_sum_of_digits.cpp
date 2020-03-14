#include "sum_of_digits.h"
#include <gtest/gtest.h>

namespace {

struct tcase {
  int a, b, expected;
};

TEST(sum_of_digits, typical_case) {
  tcase tcases[] = {
      {123, 10, 6},        {0123, 8, 6},      {0x123, 16, 6},
      {123456789, 10, 45}, {01234567, 8, 28}, {0x1234567, 16, 28},
  };
  for (auto const &t : tcases) {
    EXPECT_EQ(sum_of_digits(t.a, t.b), t.expected)
        << "(a,b)=(" << t.a << "," << t.b << ")";
  }
}

TEST(sum_of_digits, huge_base) {

  tcase tcases[] = {
      {123456789, 2, 16},    {123456789, 4, 21},   {123456789, 9, 45},
      {123456789, 16, 54},   {123456789, 25, 69},  {123456789, 36, 64},
      {123456789, 49, 117},  {123456789, 64, 162}, {123456789, 81, 229},
      {123456789, 100, 225},
  };
  for (auto const &t : tcases) {
    EXPECT_EQ(sum_of_digits(t.a, t.b), t.expected)
        << "(a,b)=(" << t.a << "," << t.b << ")";
  }
}

TEST(sum_of_digits, throw_if_invaldparam) {
  EXPECT_THROW({ sum_of_digits(100, 0); }, std::runtime_error);
  EXPECT_THROW({ sum_of_digits(100, 1); }, std::runtime_error);
  EXPECT_NO_THROW({ sum_of_digits(100, 2); });
  EXPECT_THROW({ sum_of_digits(-1, 10); }, std::runtime_error);
  EXPECT_THROW({ sum_of_digits(-123, 10); }, std::runtime_error);
  EXPECT_THROW({ sum_of_digits(123, -10); }, std::runtime_error);
  EXPECT_THROW({ sum_of_digits(-123, -10); }, std::runtime_error);
  EXPECT_NO_THROW({ sum_of_digits(0, 2); });
}
} // namespace
