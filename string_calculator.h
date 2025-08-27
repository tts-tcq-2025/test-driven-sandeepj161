#ifndef STRING_CALCULATOR_H_
#define STRING_CALCULATOR_H_

#include <stdexcept>  // std::runtime_error
#include <string>     // std::string
#include <utility>    // std::pair
#include <vector>     // std::vector

class NegativeNumberException : public std::runtime_error {
 public:
  explicit NegativeNumberException(const std::string& message)
      : std::runtime_error(message) {}
};

class StringCalculator {
 public:
  // Aligns with Python: add(str_input) -> int
  int add(const std::string& input);

 private:
  // Extensible helpers for future tasks (kept small to maintain CCN <= 3)
  bool hasCustomDelimiter(const std::string& input) const;
  std::pair<std::string, std::string> splitHeaderAndNumbers(
      const std::string& input) const;

  std::vector<std::string> extractDelimiters(const std::string& header) const;

  std::vector<std::string> tokenize(
      const std::string& payload, const std::vector<std::string>& delimiters) const;

  std::vector<int> parseNumbers(const std::vector<std::string>& tokens) const;

  void validateNoNegatives(const std::vector<int>& numbers) const;
};

#endif  // STRING_CALCULATOR_H_
