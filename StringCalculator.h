#ifndef STRING_CALCULATOR_H
#define STRING_CALCULATOR_H

#include <string>
#include <vector>
#include <stdexcept>

class StringCalculator {
public:
    int add(const std::string& input);

private:
    std::string getDelimiter(const std::string& input);
    std::string getNumbersPart(const std::string& input, const std::string& delimiter);
    int sumNumbers(const std::string& numbers, const std::string& delimiter);
    std::vector<std::string> split(const std::string& str, const std::string& delimiter);
};

#endif
