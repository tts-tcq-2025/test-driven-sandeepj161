#include "gtest/gtest.h"
#include "StringCalculator.h"
#include <stdexcept>

class StringCalculatorTest : public ::testing::Test {
protected:
    // Nothing to initialize since add() is static
};

// ---------------- Basic Scenarios ----------------

TEST_F(StringCalculatorTest, EmptyStringReturnsZero) {
    EXPECT_EQ(StringCalculator::add(""), 0);
}

TEST_F(StringCalculatorTest, SingleNumberReturnsSameNumber) {
    EXPECT_EQ(StringCalculator::add("1"), 1);
}

TEST_F(StringCalculatorTest, TwoNumbersReturnSum) {
    EXPECT_EQ(StringCalculator::add("1,2"), 3);
}

TEST_F(StringCalculatorTest, MultipleNumbersWithCommaAndNewline) {
    EXPECT_EQ(StringCalculator::add("1\n2,3"), 6);
}

TEST_F(StringCalculatorTest, NewlinesOnly) {
    EXPECT_EQ(StringCalculator::add("1\n2\n3"), 6);
}

// ---------------- Negative Numbers ----------------

TEST_F(StringCalculatorTest, SingleNegativeThrows) {
    try {
        StringCalculator::add("1,-2");
        FAIL() << "Exception expected";
    } catch (const std::runtime_error& e) {
        EXPECT_EQ(e.what(), std::string("negatives not allowed: -2"));
    }
}

TEST_F(StringCalculatorTest, MultipleNegativesThrow) {
    try {
        StringCalculator::add("1,-2,-3");
        FAIL() << "Exception expected";
    } catch (const std::runtime_error& e) {
        EXPECT_EQ(e.what(), std::string("negatives not allowed: -2, -3"));
    }
}

// ---------------- Numbers > 1000 ----------------

TEST_F(StringCalculatorTest, NumbersGreaterThan1000Ignored) {
    EXPECT_EQ(StringCalculator::add("2,1001"), 2);
}

TEST_F(StringCalculatorTest, BoundaryAt1000Included) {
    EXPECT_EQ(StringCalculator::add("2,1000"), 1002);
}

TEST_F(StringCalculatorTest, AllNumbersAbove1000ReturnZero) {
    EXPECT_EQ(StringCalculator::add("1001,2000"), 0);
}

// ---------------- Custom Delimiters ----------------

TEST_F(StringCalculatorTest, CustomSingleCharDelimiter) {
    EXPECT_EQ(StringCalculator::add("//;\n1;2"), 3);
}

TEST_F(StringCalculatorTest, CustomMultiCharDelimiter) {
    EXPECT_EQ(StringCalculator::add("//[***]\n1***2***3"), 6);
}

TEST_F(StringCalculatorTest, MultiCharDelimiterWithSpaces) {
    EXPECT_EQ(StringCalculator::add("//[ab cd]\n1ab cd2ab cd3"), 6);
}

TEST_F(StringCalculatorTest, CustomDelimiterWithDefaultSeparators) {
    EXPECT_EQ(StringCalculator::add("//;\n1;2,3"), 6);
}

TEST_F(StringCalculatorTest, CustomDelimiterOnlyWithoutNumbersReturnsZero) {
    EXPECT_EQ(StringCalculator::add("//;\n"), 0);
}

// ---------------- Mixed scenarios ----------------

TEST_F(StringCalculatorTest, MixtureOfDelimiters) {
    EXPECT_EQ(StringCalculator::add("//;\n1;2\n3,1001"), 6);
}
