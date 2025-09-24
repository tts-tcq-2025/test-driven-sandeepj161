#include "StringCalculator.h"
#include <iostream>
#include <stdexcept>

// -----------------------------
// Minimal Test Framework Macros
// -----------------------------
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
// Helper Function to Reduce Duplication
// -----------------------------
bool expectAdd(StringCalculator& calc, const std::string& input, int expected) {
    int result = calc.add(input);
    if (result != expected) {
        std::cerr << "FAILED: input=\"" << input
                  << "\", expected=" << expected
                  << ", got=" << result << std::endl;
        return false;
    }
    return true;
}

// -----------------------------
// Test Cases
// -----------------------------
bool testEmptyString() {
    StringCalculator calc;
    return expectAdd(calc, "", 0);
}

bool testSingleNumber() {
    StringCalculator calc;
    return expectAdd(calc, "1", 1);
}

bool testTwoNumbers() {
    StringCalculator calc;
    return expectAdd(calc, "1,2", 3);
}

bool testMultipleNumbers() {
    StringCalculator calc;
    return expectAdd(calc, "1,2,3,4", 10);
}

bool testNewlineDelimiter() {
    StringCalculator calc;
    return expectAdd(calc, "1\n2,3", 6);
}

bool testCustomDelimiter() {
    StringCalculator calc;
    return expectAdd(calc, "//;\n1;2", 3);
}

bool testMultiCharDelimiter() {
    StringCalculator calc;
    return expectAdd(calc, "//[***]\n1***2***3", 6);
}

bool testMultipleDelimiters() {
    StringCalculator calc;
    return expectAdd(calc, "//[*][%]\n1*2%3", 6);
}

bool testNegativeNumbers() {
    StringCalculator calc;
    EXPECT_THROW(calc.add("1,-2,3"), NegativeNumberException);
    EXPECT_THROW(calc.add("1,-2,-3"), NegativeNumberException);
    EXPECT_THROW(calc.add("-1,2,3"), NegativeNumberException);
    return true;
}

bool testIgnoreLargeNumbers() {
    StringCalculator calc;
    return expectAdd(calc, "2,1001", 2) &&
           expectAdd(calc, "2,1000", 1002) &&
           expectAdd(calc, "1001,2000", 0);
}

bool testCustomDelimiterWithMixedValues() {
    StringCalculator calc;
    return expectAdd(calc, "//;\n1;2,3", 6) &&
           expectAdd(calc, "//[ab cd]\n1ab cd2ab cd3", 6) &&
           expectAdd(calc, "//|\n1|2|3", 6) &&
           expectAdd(calc, "//;\n7", 7);
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
    if (!testCustomDelimiterWithMixedValues()) return 1;

    std::cout << "All tests passed successfully!" << std::endl;
    return 0;
}
