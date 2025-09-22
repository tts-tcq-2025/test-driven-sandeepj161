#pragma once
#include <string>
#include <vector>

class StringCalculator {
public:
    static int add(const std::string& input);

private:
    struct Parsed {
        std::string numbers;
        std::vector<std::string> delimiters;
    };

    static bool isBlank(const std::string& input);
    static Parsed parseInput(const std::string& input);
    static std::vector<std::string> parseCustomDelimiters(const std::string& part);
    static std::vector<int> splitNumbers(const std::string& numbers, const std::vector<std::string>& delimiters);
    static void validateNoNegatives(const std::vector<int>& numbers);
    static int calculateSum(const std::vector<int>& numbers);
};
