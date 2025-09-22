#pragma once
#include <string>
#include <vector>

class StringCalculator {
public:
    int add(const std::string& numbers);

private:
    std::vector<std::string> extractDelimiters(const std::string& numbers, std::string& numbersLine);
    std::vector<std::string> parseMultiCharDelimiters(const std::string& header);
    std::vector<int> splitNumbers(const std::string& numbersLine, const std::vector<std::string>& delimiters);
    void validateNegatives(const std::vector<int>& numbers);
    int sumNumbers(const std::vector<int>& numbers);
    bool isNegative(int number);
    bool isValidNumber(int number);
    const std::vector<std::string> DEFAULT_DELIMITERS = {",", "\n"};
};
