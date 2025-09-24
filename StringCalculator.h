#ifndef STRINGCALCULATOR_H_
#define STRINGCALCULATOR_H_

// Tell cppcheck this is C++ code
#if !defined(__cplusplus)
#error "This header requires C++"
#endif

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
    int add(const std::string& numbers);

 private:
    std::vector<std::string> split(const std::string& input,
                                   const std::vector<std::string>& delimiters);
    std::vector<std::string> extractDelimiters(const std::string& input,
                                               std::string& numbersPart);
    std::vector<int> toNumbers(const std::vector<std::string>& tokens);
};

#endif  // STRINGCALCULATOR_H_
