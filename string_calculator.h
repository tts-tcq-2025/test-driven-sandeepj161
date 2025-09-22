#ifndef STRING_CALCULATOR_H
#define STRING_CALCULATOR_H

#include <stdexcept>
#include <string>
#include <vector>

class NegativeNumberException : public std::runtime_error {
public:
    explicit NegativeNumberException(const std::vector<int>& negatives);
    const std::vector<int>& getNegatives() const;

private:
    std::vector<int> negatives_;
};

class StringCalculator {
public:
    int add(const std::string& input);

private:
    // --- Parsing helpers ---
    std::vector<std::string> extractDelimiters(const std::string& header, bool& valid);
    std::string extractHeader(const std::string& input, bool& hasHeader, size_t& numbersStart);
    std::vector<std::string> split(const std::string& str, const std::vector<std::string>& delimiters);

    // --- Number handling ---
    int sumNumbers(const std::vector<std::string>& tokens);
    bool isNumber(const std::string& token);
};

#endif // STRING_CALCULATOR_H
