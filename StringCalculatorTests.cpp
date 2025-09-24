#include "StringCalculator.h"
#include <iostream>
#include <stdexcept>

// -----------------------------
// Minimal Test Framework Macros
// -----------------------------
#define EXPECT_EQ(actual, expected) \
    do { \
        auto actVal = (actual); \
        auto expVal = (expected); \
        if (actVal != expVal) { \
            std::cerr << "FAILED: " << __FILE__ << ":" << __LINE__ \
                      << " Expected: " << expVal \
                      << " Got: " << actVal << std::endl; \
            return false; \
        } \
    } while (0)

#define EXPECT_THROW(statement, exceptionType) \
    do { \
        bool thrown = false; \
        try { \
            statement; \
        } catch (const exceptionType&) { \
            thrown = true; \
        } catch (...) { \
            std::cerr << "FAILED: " << __FILE__ << ":" << __LINE__ \
                      << " Unexpected exception type thrown." << std::endl; \
            return false; \
        } \
        if (!thrown) { \
            std::cerr << "FAILED: " << __FILE__ << ":" << __LINE__ \
                      << " Expected exception not thrown." << std::endl; \
            return false; \
        } \
    } while (0)

// -----------------------------
// Test Cases
// -----------------------------
bool testEmptyString() {
    StringCalculator calc;
    auto result = calc.add("");
    EXPECT_EQ(result, 0);
    return true;
}

bool testSingleNumber() {
    StringCalculator calc;
    auto result = calc.add("1");
    EXPECT_EQ(result, 1);
    return true;
}

bool testTwoNumbers() {
    StringCalculator calc;
    auto result = calc.add("1,2");
    EXPECT_EQ(result, 3);
    return true;
}

bool testMultipleNumbers() {
    StringCalculator calc;
    auto result = calc.add("1,2,3,4");
    EXPECT_EQ(result, 10);
    return true;
}

bool testNewlineDelimiter() {
    StringCalculator calc;
    auto result = calc.add("1\n2,3");
    EXPECT_EQ(result, 6);
    return true;
}

bool testCustomDelimiter() {
    StringCalculator calc;
    auto result = calc.add("//;\n1;2");
    EXPECT_EQ(result, 3);
    return true;
}

bool testMultiCharDelimiter() {
    StringCalculator calc;
    auto result = calc.add("//[***]\n1***2***3");
    EXPECT_EQ(result, 6);
    return true;
}

bool testMultipleDelimiters() {
    StringCalculator calc;
    auto result = calc.add("//[*][%]\n1*2%3");
    EXPECT_EQ(result, 6);
    return true;
}

bool testNegativeNumbers() {
    StringCalculator calc;
    EXPECT_THROW(calc.add("1,-2,3"), NegativeNumberException);
    return true;
}

bool testIgnoreLargeNumbers() {
    StringCalculator calc;
    auto result1 = calc.add("2,1001");
    EXPECT_EQ(result1, 2);
    auto result2 = calc.add("2,1000");
    EXPECT_EQ(result2, 1002);
    return true;
}

// -----------------------------
// Main Test Runner
// -----------------------------
int main() {
    if (!testEmptyString()) return 1;
    if (!testSingleNumber()) return 1;
    if (!testTwoNumbers()) return 1;
    if (!testMultipleNumbers()) return 1;
    if (!testNewlineDelimiter()) return 1;
    if (!testCustomDelimiter()) return 1;
    if (!testMultiCharDelimiter()) return 1;
    if (!testMultipleDelimiters()) return 1;
    if (!testNegativeNumbers()) return 1;
    if (!testIgnoreLargeNumbers()) return 1;

    std::cout << "All tests passed successfully!" << std::endl;
    return 0;
}
