#include "StringCalculator.h"
#include <sstream>
#include <regex>
#include <stdexcept>
#include <algorithm>

int StringCalculator::add(const std::string& numbers) {
    if (numbers.empty()) return 0;

    std::string numbersLine;
    auto delimiters = extractDelimiters(numbers, numbersLine);
    auto numList = splitNumbers(numbersLine, delimiters);
    validateNegatives(numList);
    return sumNumbers(numList);
}

// ---------------- Helper Methods ----------------

std::vector<std::string> StringCalculator::extractDelimiters(const std::string& numbers, std::string& numbersLine) {
    numbersLine = numbers;
    if (numbers.rfind("//", 0) != 0) return DEFAULT_DELIMITERS;

    size_t newlinePos = numbers.find('\n');
    std::string header = numbers.substr(0, newlinePos);
    numbersLine = numbers.substr(newlinePos + 1);

    auto multiDelims = parseMultiCharDelimiters(header);
    if (!multiDelims.empty()) return multiDelims;

    if (header.length() > 2) return {header.substr(2)};
    return DEFAULT_DELIMITERS;
}

std::vector<std::string> StringCalculator::parseMultiCharDelimiters(const std::string& header) {
    std::regex multiCharRegex("\\[(.*?)\\]");
    std::sregex_iterator iter(header.begin(), header.end(), multiCharRegex);
    std::sregex_iterator end;

    std::vector<std::string> delimiters;
    for (; iter != end; ++iter) {
        delimiters.push_back((*iter)[1].str());
    }
    return delimiters;
}

std::vector<int> StringCalculator::splitNumbers(const std::string& numbersLine, const std::vector<std::string>& delimiters) {
    std::string regexPattern;
    for (const auto& d : delimiters) {
        if (!regexPattern.empty()) regexPattern += "|";
        regexPattern += std::regex_replace(d, std::regex(R"([\^$.|?*+(){}\[\]\\])"), R"(\\$&)");
    }

    std::regex re(regexPattern);
    std::sregex_token_iterator iter(numbersLine.begin(), numbersLine.end(), re, -1);
    std::sregex_token_iterator end;

    std::vector<int> numbers;
    for (; iter != end; ++iter) {
        std::string token = iter->str();
        if (token.empty()) continue;
        numbers.push_back(std::stoi(token));
    }
    return numbers;
}

void StringCalculator::validateNegatives(const std::vector<int>& numbers) {
    std::vector<int> negatives;
    for (int n : numbers) if (isNegative(n)) negatives.push_back(n);

    if (!negatives.empty()) {
        std::string msg = "negatives not allowed: ";
        for (size_t i = 0; i < negatives.size(); ++i) {
            msg += std::to_string(negatives[i]);
            if (i < negatives.size() - 1) msg += ", ";
        }
        throw std::runtime_error(msg);
    }
}

int StringCalculator::sumNumbers(const std::vector<int>& numbers) {
    int sum = 0;
    for (int n : numbers) if (isValidNumber(n)) sum += n;
    return sum;
}

bool StringCalculator::isNegative(int number) {
    return number < 0;
}

bool StringCalculator::isValidNumber(int number) {
    return number <= 1000;
}
