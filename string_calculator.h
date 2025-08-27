#ifndef STRING_CALCULATOR_H_
#define STRING_CALCULATOR_H_

#include <stdexcept>
#include <string>

class NegativeNumberException : public std::runtime_error {
 public:
  explicit NegativeNumberException(const std::string& message)
      : std::runtime_error(message) {}
};

class StringCalculator {
 public:
  // Mirrors Python: add(str_input) -> int
  int add(const std::string& input);
};

#endif  // STRING_CALCULATOR_H_
