#include <gtest/gtest.h>
#include <string>

#include "./string_calculator.h"

class StringCalculatorTest : public ::testing::Test {
 protected:
  StringCalculator calc;
};

TEST_F(StringCalculatorTest, EmptyStringReturnsZero) {
  EXPECT_EQ(calc.add(""), 0);
}

TEST_F(StringCalculatorTest, SingleNumberReturnsValue) {
  EXPECT_EQ(calc.add("5"), 5);
}

TEST_F(StringCalculatorTest, TwoNumbersCommaSeparated) {
  EXPECT_EQ(calc.add("1,2"), 3);
}

TEST_F(StringCalculatorTest, MultipleNumbersCommaSeparated) {
  EXPECT_EQ(calc.add("1,2,3,4"), 10);
}

TEST_F(StringCalculatorTest, NewlineAsSeparator) {
  EXPECT_EQ(calc.add("1\n2,3"), 6);
}

TEST_F(StringCalculatorTest, CustomSingleCharDelimiter) {
  EXPECT_EQ(calc.add("//;\n1;2"), 3);
}

TEST_F(StringCalculatorTest, NumbersGreaterThan1000Ignored) {
  EXPECT_EQ(calc.add("2,1001"), 2);
}

TEST_F(StringCalculatorTest, ThrowsOnNegativeNumbers) {
  EXPECT_THROW(calc.add("1,-2,3"), NegativeNumberException);
}

TEST_F(StringCalculatorTest, ExceptionMessageListsNegatives) {
  try {
    calc.add("2,-4,3,-5");
    FAIL() << "Expected NegativeNumberException";
  } catch (const NegativeNumberException& e) {
    std::string msg = e.what();
    EXPECT_NE(msg.find("-4"), std::string::npos);
    EXPECT_NE(msg.find("-5"), std::string::npos);
  }
}
