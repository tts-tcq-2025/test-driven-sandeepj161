#include "gtest/gtest.h"
#include "StringCalculator.h"
#include <stdexcept>

class StringCalculatorTest : public ::testing::Test {
protected:
    StringCalculator calc;
};

// ---------------- Basic Scenarios ----------------

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

TEST_F(StringCalculatorTest, NewlineAsDelimiter) {
    EXPECT_EQ(calc.add("1\n2,3"), 6);
}

// ---------------- Custom Delimiters ----------------

TEST_F(StringCalculatorTest, CustomSingleCharDelimiter) {
    EXPECT_EQ(calc.add("//;\n1;2"), 3);
}

TEST_F(StringCalculatorTest, CustomMultiCharDelimiter) {
    EXPECT_EQ(calc.add("//[***]\n1***2***3"), 6);
}

TEST_F(StringCalculatorTest, MultiCharDelimiterWithSpaces) {
    EXPECT_EQ(calc.add("//[ab cd]\n1ab cd2ab cd3"), 6);
}

TEST_F(StringCalculatorTest, SingleValueWithCustomDelimiter) {
    EXPECT_EQ(calc.add("//;\n7"), 7);
}

TEST_F(StringCalculatorTest, CustomDelimiterWithDefaultSeparators) {
    EXPECT_EQ(calc.add("//;\n1;2,3"), 6);
}

// ---------------- Negative Numbers ----------------

TEST_F(StringCalculatorTest, SingleNegativeThrows) {
    EXPECT_THROW({
        try {
            calc.add("1,-2,3");
        } catch (const std::runtime_error& e) {
            EXPECT_STREQ(e.what(), "negatives not allowed: -2");
            throw;
        }
    }, std::runtime_error);
}

TEST_F(StringCalculatorTest, MultipleNegativesThrow) {
    EXPECT_THROW({
        try {
            calc.add("1,-2,-3");
        } catch (const std::runtime_error& e) {
            EXPECT_STREQ(e.what(), "negatives not allowed: -2, -3");
            throw;
        }
    }, std::runtime_error);
}

TEST_F(StringCalculatorTest, NegativeAtStartThrows) {
    EXPECT_THROW({
        try {
            calc.add("-1,2,3");
        } catch (const std::runtime_error& e) {
            EXPECT_STREQ(e.what(), "negatives not allowed: -1");
            throw;
        }
    }, std::runtime_error);
}

// ---------------- Numbers >1000 ----------------

TEST_F(StringCalculatorTest, NumbersGreaterThan1000AreIgnored) {
    EXPECT_EQ(calc.add("2,1001"), 2);
}

TEST_F(StringCalculatorTest, BoundaryAt1000IsIncluded) {
    EXPECT_EQ(calc.add("2,1000"), 1002);
}

TEST_F(StringCalculatorTest, AllValuesAbove1000YieldZero) {
    EXPECT_EQ(calc.add("1001,2000"), 0);
}

// ---------------- Mixed Scenarios ----------------

TEST_F(StringCalculatorTest, MixedDefaultDelimitersWithoutHeader) {
    EXPECT_EQ(calc.add("4\n5,6"), 15);
}

TEST_F(StringCalculatorTest, HeaderOptional) {
    EXPECT_EQ(calc.add("1,2,3"), 6);
}

// ---------------- Delimiter Format Validation ----------------

TEST_F(StringCalculatorTest, HeaderNotAtStartIgnored) {
    EXPECT_EQ(calc.add("1,2\n//;\n3;4"), 10);
}

TEST_F(StringCalculatorTest, EmptyDelimiterBracketsUsesDefault) {
    EXPECT_EQ(calc.add("//[]\n1,2"), 3);
}

TEST_F(StringCalculatorTest, CustomSingleCharDelimiterMultipleValues) {
    EXPECT_EQ(calc.add("//|\n1|2|3"), 6);
}

// Clarification-only scenarios (no validation required)
TEST_F(StringCalculatorTest, TrailingDelimiterClarification) {
    EXPECT_NO_THROW(calc.add("1,\n"));
}
