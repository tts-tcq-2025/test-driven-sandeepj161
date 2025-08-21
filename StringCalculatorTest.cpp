#include <gtest/gtest.h>
#include "StringCalculator.h"

class StringCalculatorTest : public ::testing::Test {
 protected:
  StringCalculator calculator;
};

TEST_F(StringCalculatorTest, EmptyStringReturnsZero) {
  EXPECT_EQ(0, calculator.add(""));
}

TEST_F(StringCalculatorTest, SingleNumberReturnsItself) {
  EXPECT_EQ(1, calculator.add("1"));
  EXPECT_EQ(5, calculator.add("5"));
}

TEST_F(StringCalculatorTest, TwoNumbersSummed) {
  EXPECT_EQ(3, calculator.add("1,2"));
}

TEST_F(StringCalculatorTest, MultipleNumbersSummed) {
  EXPECT_EQ(6, calculator.add("1,2,3"));
  EXPECT_EQ(10, calculator.add("1,2,3,4"));
}

TEST_F(StringCalculatorTest, NewLinesAsDelimiters) {
  EXPECT_EQ(6, calculator.add("1\n2,3"));
  EXPECT_EQ(6, calculator.add("1,2\n3"));
}

TEST_F(StringCalculatorTest, CustomDelimiter) {
  EXPECT_EQ(3, calculator.add("//;\n1;2"));
  EXPECT_EQ(6, calculator.add("//|\n1|2|3"));
}

TEST_F(StringCalculatorTest, NegativeNumbersThrowException) {
  EXPECT_THROW(calculator.add("1,-2"), std::invalid_argument);
  try {
    calculator.add("1,-2");
  } catch (const std::invalid_argument& e) {
    EXPECT_STREQ("negatives not allowed: -2", e.what());
  }

  EXPECT_THROW(calculator.add("1,-2,-3"), std::invalid_argument);
  try {
    calculator.add("1,-2,-3");
  } catch (const std::invalid_argument& e) {
    EXPECT_STREQ("negatives not allowed: -2, -3", e.what());
  }
}

TEST_F(StringCalculatorTest, IgnoreNumbersOver1000) {
  EXPECT_EQ(2, calculator.add("2,1001"));
  EXPECT_EQ(1002, calculator.add("2,1000"));
  EXPECT_EQ(3, calculator.add("1,2,1001"));
}

TEST_F(StringCalculatorTest, MultiCharacterDelimiters) {
  EXPECT_EQ(6, calculator.add("//[***]\n1***2***3"));
  EXPECT_EQ(6, calculator.add("//[abc]\n1abc2abc3"));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
