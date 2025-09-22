#include <gtest/gtest.h>
#include <string>
#include "./string_calculator.h"

class StringCalculatorTest : public ::testing::Test {
 protected:
  StringCalculator calc;
};

// --- Core Spec Tests ---

TEST_F(StringCalculatorTest, EmptyInputReturnsZero) {
  EXPECT_EQ(calc.add(""), 0);
}

TEST_F(StringCalculatorTest, SingleNumberReturnsItsValue) {
  EXPECT_EQ(calc.add("1"), 1);
}

TEST_F(StringCalculatorTest, TwoNumbersCommaSeparated) {
  EXPECT_EQ(calc.add("1,2"), 3);
}

TEST_F(StringCalculatorTest, ArbitraryCountOfNumbers) {
  EXPECT_EQ(calc.add("1,2,3,4"), 10);
}

TEST_F(StringCalculatorTest, NewlinesAsDelimiters) {
  EXPECT_EQ(calc.add("1\n2,3"), 6);
}

TEST_F(StringCalculatorTest, CustomSingleCharacterDelimiter) {
  EXPECT_EQ(calc.add("//;\n1;2"), 3);
}

TEST_F(StringCalculatorTest, CustomMultiCharacterDelimiter) {
  EXPECT_EQ(calc.add("//[***]\n1***2***3"), 6);
}

TEST_F(StringCalculatorTest, NegativeNumberThrowsSingle) {
  EXPECT_THROW({
    try {
      calc.add("1,-2,3");
    } catch (const NegativeNumberException& e) {
      EXPECT_STREQ(e.what(), "Negatives not allowed: -2");
      throw;
    }
  }, NegativeNumberException);
}

TEST_F(StringCalculatorTest, NegativeNumberThrowsMultiple) {
  EXPECT_THROW({
    try {
      calc.add("1,-2,-3");
    } catch (const NegativeNumberException& e) {
      EXPECT_STREQ(e.what(), "Negatives not allowed: -2, -3");
      throw;
    }
  }, NegativeNumberException);
}

TEST_F(StringCalculatorTest, NumbersGreaterThan1000AreIgnored) {
  EXPECT_EQ(calc.add("2,1001"), 2);
}

TEST_F(StringCalculatorTest, Boundary1000IsIncluded) {
  EXPECT_EQ(calc.add("2,1000"), 1002);
}

TEST_F(StringCalculatorTest, HeaderIsOptional) {
  EXPECT_EQ(calc.add("1,2,3"), 6);
}

TEST_F(StringCalculatorTest, CustomHeaderDefinesDelimiter) {
  EXPECT_EQ(calc.add("//;\n1;2,3"), 6);
}

TEST_F(StringCalculatorTest, MultiCharacterDelimiterWithSpaces) {
  EXPECT_EQ(calc.add("//[ab cd]\n1ab cd2ab cd3"), 6);
}

TEST_F(StringCalculatorTest, MixedDefaultDelimitersWithoutHeader) {
  EXPECT_EQ(calc.add("4\n5,6"), 15);
}

TEST_F(StringCalculatorTest, CustomDelimiterMultipleValues) {
  EXPECT_EQ(calc.add("//|\n1|2|3"), 6);
}

TEST_F(StringCalculatorTest, SingleValueWithCustomDelimiter) {
  EXPECT_EQ(calc.add("//;\n7"), 7);
}

TEST_F(StringCalculatorTest, AllValuesGreaterThan1000YieldZero) {
  EXPECT_EQ(calc.add("1001,2000"), 0);
}

TEST_F(StringCalculatorTest, NegativeAtStartThrows) {
  EXPECT_THROW({
    try {
      calc.add("-1,2,3");
    } catch (const NegativeNumberException& e) {
      EXPECT_STREQ(e.what(), "Negatives not allowed: -1");
      throw;
    }
  }, NegativeNumberException);
}

// --- Delimiter Format Validation Tests ---

TEST_F(StringCalculatorTest, HeaderNotAtStartIsIgnored) {
  EXPECT_EQ(calc.add("1,2\n//;\n3;4"), 10);
}

TEST_F(StringCalculatorTest, MissingNewlineAfterHeaderIsIgnored) {
  EXPECT_EQ(calc.add("//;1;2"), 0);  // undefined → treat as invalid header, return 0
}

TEST_F(StringCalculatorTest, EmptyDelimiterBracketsTreatedAsDefault) {
  EXPECT_EQ(calc.add("//[]\n1,2"), 3);
}

TEST_F(StringCalculatorTest, MultiCharDelimiterBracketed) {
  EXPECT_EQ(calc.add("//[***]\n1***2***3"), 6);
}

TEST_F(StringCalculatorTest, MultiCharDelimiterWithoutBracketsInvalid) {
  EXPECT_EQ(calc.add("//***\n1***2***3"), 0);
}

TEST_F(StringCalculatorTest, SingleCharCustomDelimiter) {
  EXPECT_EQ(calc.add("//;\n1;2;3"), 6);
}

TEST_F(StringCalculatorTest, HeaderWithSpacesInsideDelimiter) {
  EXPECT_EQ(calc.add("//[ab cd]\n1ab cd2ab cd3"), 6);
}

TEST_F(StringCalculatorTest, MixedWithCustomHeaderInPayload) {
  EXPECT_EQ(calc.add("//;\n1;2,3"), 6);
}

TEST_F(StringCalculatorTest, TrailingDelimiterClarificationCase) {
  EXPECT_NO_THROW(calc.add("1,\n"));  // spec: no validation required
}

TEST_F(StringCalculatorTest, MultipleHeadersNotSupported) {
  EXPECT_EQ(calc.add("//;\n//|\n1|2"), 0);
}

TEST_F(StringCalculatorTest, HeaderWithMissingClosingBracketInvalid) {
  EXPECT_EQ(calc.add("//[**\n1**2**3"), 0);
}

TEST_F(StringCalculatorTest, EmptyNumbersLineAfterHeaderReturnsZero) {
  EXPECT_EQ(calc.add("//;\n"), 0);
}

TEST_F(StringCalculatorTest, WhitespaceInsideDelimiterIsPartOfIt) {
  EXPECT_EQ(calc.add("//[  **  ]\n1  **  2"), 3);
}

TEST_F(StringCalculatorTest, DefaultDelimitersStillWorkWithCustomHeader) {
  EXPECT_EQ(calc.add("//;\n1;2,3"), 6);
}
