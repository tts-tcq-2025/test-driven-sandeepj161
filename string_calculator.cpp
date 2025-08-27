#include "./string_calculator.h"

#include <string>

// Minimal, pure, and TDD-friendly: only implement what's tested.
int StringCalculator::add(const std::string& input) {
  if (input.empty()) return 0;
  // Future tests will drive additional behavior here (e.g., parsing, delimiters).
  return 0;  //  Default for now until specs/tests require more.
}
