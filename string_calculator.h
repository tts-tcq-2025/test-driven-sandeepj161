#ifndef STRING_CALCULATOR_H_
#define STRING_CALCULATOR_H_

#include <stdexcept>
#include <string>
#include <vector>

// Custom exception for negative numbers
class NegativeNumberException : public std::runtime_error {
 public:
  explicit NegativeNumberException(const std::string& message)
      : std::runtime_error(message) {}
};

class StringCalculator {
 public:
  // Main entry point
  int add(const std::string& input);

 private:
  // Helpers
  std::vector<std::string> tokenize(const std::string& input);
  std::vector<int> convertToNumbers(const std::vector<std::string>& tokens);
  void validateNumbers(const std::vector<int>& numbers);
  int sumNumbers(const std::vector<int>& numbers);

  // Constants
  static constexpr int kMaxNumber = 1000;
};

#endif  // STRING_CALCULATOR_H_
