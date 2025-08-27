#include "./string_calculator.h"
#include <string>
#include <gtest/gtest.h>

class StringCalculatorTest : public ::testing::Test {
 protected:
  StringCalculator calc;
};

TEST_F(StringCalculatorTest, EmptyStringReturnZero) {
  std::string input = "";
  int expected_value = 0;
  int actual_value = calc.add(input);
  EXPECT_EQ(actual_value, expected_value);
}
