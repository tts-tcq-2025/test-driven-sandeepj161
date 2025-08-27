#include "./string_calculator.h"
#include <string>        //  std::string
#include <gtest/gtest.h>  //  GoogleTest

class StringCalculatorTest : public ::testing::Test {
 protected:
  StringCalculator calc;
};

TEST_F(StringCalculatorTest, EmptyStringReturnZero) {
  // arrange
  std::string input = "";
  int expected_value = 0;
  // act
  int actual_value = calc.add(input);
  // assert
  EXPECT_EQ(actual_value, expected_value);
}
