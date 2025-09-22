#ifndef STRING_CALCULATOR_H_
#define STRING_CALCULATOR_H_

#include <stdexcept>
#include <string>
#include <vector>

class NegativeNumberException : public std::runtime_error {
 public:
  explicit NegativeNumberException(const std::string& message)
      : std::runtime_error(message) {}
};

class StringCalculator {
 public:
  int add(const std::string& input);

 private:
  std::pair<std::vector<std::string>, std::string> parseHeader(const std::string& input) const;
  std::vector<int> tokenize(const std::string& input,
                            const std::vector<std::string>& delimiters) const;
  int computeSum(const std::vector<int>& numbers) const;
};

#endif
