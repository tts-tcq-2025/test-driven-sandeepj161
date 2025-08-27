#include "string_calculator.h"
#include <gtest/gtest.h>

class StringCalculatorTest : public ::testing::Test {
protected:
    StringCalculator calc;
};

// --- Basics ---
TEST_F(StringCalculatorTest, EmptyInputReturnsZero) { EXPECT_EQ(calc.add(""), 0); }
TEST_F(StringCalculatorTest, SingleNumberReturnsItsValue) { EXPECT_EQ(calc.add("1"), 1); }
TEST_F(StringCalculatorTest, TwoNumbersSum) { EXPECT_EQ(calc.add("1,2"), 3); }
TEST_F(StringCalculatorTest, MultipleNumbers) { EXPECT_EQ(calc.add("1,2,3,4"), 10); }

// --- Delimiters ---
TEST_F(StringCalculatorTest, HandlesNewlineDelimiter) { EXPECT_EQ(calc.add("1\n2,3"), 6); }
TEST_F(StringCalculatorTest, CustomSingleCharDelimiter) { EXPECT_EQ(calc.add("//;\n1;2"), 3); }
TEST_F(StringCalculatorTest, CustomMultiCharDelimiter) { EXPECT_EQ(calc.add("//[***]\n1***2***3"), 6); }
TEST_F(StringCalculatorTest, CustomDelimiterWithSpaces) { EXPECT_EQ(calc.add("//[ab cd]\n1ab cd2ab cd3"), 6); }
TEST_F(StringCalculatorTest, MixedDefaultDelimiters) { EXPECT_EQ(calc.add("4\n5,6"), 15); }
TEST_F(StringCalculatorTest, CustomHeaderWithCommasStillWorks) { EXPECT_EQ(calc.add("//;\n1;2,3"), 6); }

// --- Negatives ---
TEST_F(StringCalculatorTest, NegativeThrowsSingle) {
    try { calc.add("1,-2,3"); FAIL() << "Expected NegativeNumberException"; }
    catch (const NegativeNumberException& e) { EXPECT_STREQ("negatives not allowed: -2", e.what()); }
}
TEST_F(StringCalculatorTest, NegativeThrowsMultiple) {
    try { calc.add("1,-2,-3"); FAIL() << "Expected NegativeNumberException"; }
    catch (const NegativeNumberException& e) { EXPECT_STREQ("negatives not allowed: -2, -3", e.what()); }
}
TEST_F(StringCalculatorTest, NegativeAtStartThrows) {
    try { calc.add("-1,2,3"); FAIL() << "Expected NegativeNumberException"; }
    catch (const NegativeNumberException& e) { EXPECT_STREQ("negatives not allowed: -1", e.what()); }
}

// --- >1000 rule ---
TEST_F(StringCalculatorTest, NumbersAbove1000AreIgnored) { EXPECT_EQ(calc.add("2,1001"), 2); }
TEST_F(StringCalculatorTest, BoundaryAt1000Included) { EXPECT_EQ(calc.add("2,1000"), 1002); }
TEST_F(StringCalculatorTest, AllValuesAbove1000YieldZero) { EXPECT_EQ(calc.add("1001,2000"), 0); }

// --- Header/format clarifications ---
TEST_F(StringCalculatorTest, HeaderNotAtStartIsIgnored) { EXPECT_EQ(calc.add("1,2\n//;\n3;4"), 10); }
TEST_F(StringCalculatorTest, MissingNewlineAfterHeaderReturnsZero) { EXPECT_EQ(calc.add("//;1;2"), 0); }
TEST_F(StringCalculatorTest, EmptyDelimiterBracketsFallbackToDefault) { EXPECT_EQ(calc.add("//[]\n1,2"), 3); }
TEST_F(StringCalculatorTest, MultiCharacterDelimiterWithoutBracketsUndefined) { EXPECT_EQ(calc.add("//***\n1***2***3"), 0); }
