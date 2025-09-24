#include "StringCalculator.h"
#include <string>
#include <cassert>
#include <iostream>

void testEmptyInput() {
  StringCalculator calc;
  assert(calc.add("") == 0);
}

void testSingleNumber() {
  StringCalculator calc;
  assert(calc.add("1") == 1);
}

void testTwoNumbers() {
  StringCalculator calc;
  assert(calc.add("1,2") == 3);
}

void testArbitraryCount() {
  StringCalculator calc;
  assert(calc.add("1,2,3,4") == 10);
}

void testNewlines() {
  StringCalculator calc;
  assert(calc.add("1\n2,3") == 6);
}

void testCustomDelimiter() {
  StringCalculator calc;
  assert(calc.add("//;\n1;2") == 3);
}

void testMultiCharDelimiter() {
  StringCalculator calc;
  assert(calc.add("//[***]\n1***2***3") == 6);
}

void testNegativeNumbers() {
  StringCalculator calc;
  try {
    calc.add("1,-2,-3");
    assert(false);  // Should not reach here
  } catch (const NegativeNumberException& ex) {
    std::string msg = ex.what();
    assert(msg == "negatives not allowed: -2, -3");
  }
}

void testIgnoreLargeNumbers() {
  StringCalculator calc;
  assert(calc.add("2,1001") == 2);
  assert(calc.add("2,1000") == 1002);
}

void runAllTests() {
  testEmptyInput();
  testSingleNumber();
  testTwoNumbers();
  testArbitraryCount();
  testNewlines();
  testCustomDelimiter();
  testMultiCharDelimiter();
  testNegativeNumbers();
  testIgnoreLargeNumbers();
  std::cout << "All tests passed!\n";
}

int main() {
  runAllTests();
  return 0;
}
