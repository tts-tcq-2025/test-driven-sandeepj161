#include "StringCalculator.h"
#include <string>
#include <iostream>
#include <stdexcept>

// Simple test macros
#define EXPECT_EQ(actual, expected) \
    if ((actual) != (expected)) { \
        std::cerr << "FAILED: " << __FILE__ << ":" << __LINE__ \
                  << " Expected: " << (expected) \
                  << " Got: " << (actual) << std::endl; \
        return false; \
    }

#define EXPECT_THROW(statement, exceptionType) \
    { \
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
    }

// ---- Test Cases ----
bool testEmptyString() {
    StringCalculator calc;
    EXPECT_EQ(calc.add(""), 0);
    return true;
}

bool testSingleNumber() {
    StringCalculator calc;
    EXPECT_EQ(calc.add("1"), 1);
    return true;
}

bool testTwoNumbers() {
    StringCalculator calc;
    EXPECT_EQ(calc.add("1,2"), 3);
    return true;
}

bool testMultipleNumbers() {
    StringCalculator calc;
    EXPECT_EQ(calc.add("1,2,3,4"), 10);
    return true;
}

bool testNewlineDelimiter() {
    StringCalculator calc;
    EXPECT_EQ(calc.add("1\n2,3"), 6);
    return true;
}

bool testCustomDelimiter() {
    StringCalculator calc;
    EXPECT_EQ(calc.add("//;\n1;2"), 3);
    return true;
}

bool testMultiCharDelimiter() {
    StringCalculator calc;
    EXPECT_EQ(calc.add("//[***]\n1***2***3"), 6);
    return true;
}

bool testNegativeNumbers() {
    StringCalculator calc;
    EXPECT_THROW(calc.add("1,-2,3"), NegativeNumberException);
    return true;
}

bool testIgnoreLargeNumbers() {
    StringCalculator calc;
    EXPECT_EQ(calc.add("2,1001"), 2);
    EXPECT_EQ(calc.add("2,1000"), 1002);
    return true;
}

// ---- Main Runner ----
int main() {
    if (!testEmptyString()) return 1;
    if (!testSingleNumber()) return 1;
    if (!testTwoNumbers()) return 1;
    if (!testMultipleNumbers()) return 1;
    if (!testNewlineDelimiter()) return 1;
    if (!testCustomDelimiter()) return 1;
    if (!testMultiCharDelimiter()) return 1;
    if (!testNegativeNumbers()) return 1;
    if (!testIgnoreLargeNumbers()) return 1;

    std::cout << "All tests passed successfully!" << std::endl;
    return 0;
}
