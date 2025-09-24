#ifndef STRINGCALCULATOR_H_
#define STRINGCALCULATOR_H_

#include <exception>
#include <string>
#include <vector>

class NegativeNumberException : public std::exception {
 public:
  explicit NegativeNumberException(const std::vector<int>& negatives);
  const char* what() const noexcept override;

 private:
  std::string message_;
};

class StringCalculator {
 public:
  int add(const std::string& input);

 private:
  std::vector<std::string> extractDelimiters(const std::string& input,
                                             std::string& numbersSection);
  std::vector<std::string> split(const std::string& input,
                                 const std::vector<std::string>& delimiters);
  std::vector<int> convertToNumbers(const std::vector<std::string>& tokens);
};

#endif  // STRINGCALCULATOR_H_
