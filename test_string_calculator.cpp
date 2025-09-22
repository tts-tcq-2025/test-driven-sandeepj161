#include "./string_calculator.h"
#include <gtest/gtest.h>
#include <string>

class StringCalculatorTest : public ::testing::Test {
 protected:
  StringCalculator calc;
};

// --- Basic cases ---
TEST_F(StringCalculatorTest, EmptyStringReturnZero) {
  EXPECT_EQ(calc.add(""), 0);
}

TEST_F(StringCalculatorTest, SingleNumberReturnsValue) {
  EXPECT_EQ(calc.add("1"), 1);
}

TEST_F(StringCalculatorTest, TwoNumbersCommaSeparated) {
  EXPECT_EQ(calc.add("1,2"), 3);
}

TEST_F(StringCalculatorTest, ArbitraryCountOfNumbers) {
  EXPECT_EQ(calc.add("1,2,3,4"), 10);
}

TEST_F(StringCalculatorTest, NewlineAsDelimiter) {
  EXPECT_EQ(calc.add("1\n2,3"), 6);
}

// --- Custom delimiters ---
TEST_F(StringCalculatorTest, CustomSingleCharDelimiter) {
  EXPECT_EQ(calc.add("//;\n1;2"), 3);
}

TEST_F(StringCalculatorTest, CustomMultiCharDelimiter) {
  EXPECT_EQ(calc.add("//[***]\n1***2***3"), 6);
}

TEST_F(StringCalculatorTest, MultipleCustomDelimiters) {
  EXPECT_EQ(calc.add("//[***][%%]\n1***2%%3"), 6);
}

TEST_F(StringCalculatorTest, MultiCharDelimiterWithSpaces) {
  EXPECT_EQ(calc.add("//[ab cd]\n1ab cd2ab cd3"), 6);
}

TEST_F(StringCalculatorTest, CustomHeaderMixedWithComma) {
  EXPECT_EQ(calc.add("//;\n1;2,3"), 6);
}

TEST_F(StringCalculatorTest, SingleValueWithCustomDelimiter) {
  EXPECT_EQ(calc.add("//;\n7"), 7);
}

TEST_F(StringCalculatorTest, CustomDelimiterPipe) {
  EXPECT_EQ(calc.add("//|\n1|2|3"), 6);
}

// --- Negative numbers ---
TEST_F(StringCalculatorTest, NegativeNumberThrows) {
  EXPECT_THROW({
    try {
      calc.add("1,-2,3");
    } catch (const NegativeNumberException& e) {
      EXPECT_STREQ(e.what(), "negatives not allowed: -2");
      throw;
    }
  }, NegativeNumberException);
}

TEST_F(StringCalculatorTest, MultipleNegativesThrow) {
  EXPECT_THROW({
    try {
      calc.add("1,-2,-3");
    } catch (const NegativeNumberException& e) {
      EXPECT_STREQ(e.what(), "negatives not allowed: -2, -3");
      throw;
    }
  }, NegativeNumberException);
}

TEST_F(StringCalculatorTest, NegativeAtStartThrows) {
  EXPECT_THROW({
    try {
      calc.add("-1,2,3");
    } catch (const NegativeNumberException& e) {
      EXPECT_STREQ(e.what(), "negatives not allowed: -1");
      throw;
    }
  }, NegativeNumberException);
}

// --- Numbers > 1000 ---
TEST_F(StringCalculatorTest, NumbersGreaterThan1000Ignored) {
  EXPECT_EQ(calc.add("2,1001"), 2);
}

TEST_F(StringCalculatorTest, BoundaryAt1000Included) {
  EXPECT_EQ(calc.add("2,1000"), 1002);
}

TEST_F(StringCalculatorTest, AllValuesGreaterThan1000YieldZero) {
  EXPECT_EQ(calc.add("1001,2000"), 0);
}

// --- Mixed delimiters without header ---
TEST_F(StringCalculatorTest, MixedDefaultDelimiters) {
  EXPECT_EQ(calc.add("4\n5,6"), 15);
}

// --- Header parsing fallback scenarios ---
TEST_F(StringCalculatorTest, HeaderNotAtStartIgnored) {
  EXPECT_EQ(calc.add("1,2\n//;\n3;4"), 10);  // header ignored
}

TEST_F(StringCalculatorTest, EmptyNumbersAfterHeader) {
  EXPECT_EQ(calc.add("//;\n"), 0);
}

TEST_F(StringCalculatorTest, EmptyDelimiterBracketsFallback) {
  EXPECT_EQ(calc.add("//[]\n1,2"), 3);  // fallback to default
}
