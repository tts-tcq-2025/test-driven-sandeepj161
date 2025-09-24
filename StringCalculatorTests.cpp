#include "StringCalculator.h"
#include <string>
#include <sstream>
#include <algorithm>
#include <stdexcept>

// -----------------------------
// NegativeNumberException
// -----------------------------
NegativeNumberException::NegativeNumberException(const std::vector<int>& negatives)
    : message_("negatives not allowed: ") {
    for (size_t i = 0; i < negatives.size(); ++i) {
        message_ += std::to_string(negatives[i]);
        if (i != negatives.size() - 1) {
            message_ += ", ";
        }
    }
}

const char* NegativeNumberException::what() const noexcept {
    return message_.c_str();
}

// -----------------------------
// StringCalculator Implementation
// -----------------------------
int StringCalculator::add(const std::string& numbers) {
    if (numbers.empty()) return 0;

    std::string numbersPart;
    std::vector<std::string> delimiters = extractDelimiters(numbers, numbersPart);
    std::vector<std::string> tokens = split(numbersPart, delimiters);
    std::vector<int> nums = toNumbers(tokens);

    int sum = 0;
    std::vector<int> negatives;
    for (int n : nums) {
        if (n < 0) negatives.push_back(n);
        else if (n <= 1000) sum += n;
    }

    if (!negatives.empty()) throw NegativeNumberException(negatives);

    return sum;
}

std::vector<std::string> StringCalculator::split(
        const std::string& input,
        const std::vector<std::string>& delimiters) {
    std::vector<std::string> result;
    size_t start = 0;

    for (size_t pos = 0; pos < input.size();) {
        bool matched = false;
        for (const auto& delim : delimiters) {
            if (input.compare(pos, delim.size(), delim) == 0) {
                result.push_back(input.substr(start, pos - start));
                pos += delim.size();
                start = pos;
                matched = true;
                break;
            }
        }
        if (!matched) ++pos;
    }

    if (start <= input.size()) {
        result.push_back(input.substr(start));
    }

    return result;
}

std::vector<std::string> StringCalculator::extractDelimiters(
        const std::string& input,
        std::string& numbersPart) {
    std::vector<std::string> delimiters = {",", "\n"};

    if (input.size() >= 2 && input[0] == '/' && input[1] == '/') {
        size_t newlinePos = input.find('\n');
        if (newlinePos != std::string::npos) {
            std::string header = input.substr(2, newlinePos - 2);

            // Multi-character delimiter
            if (!header.empty() && header.front() == '[' && header.back() == ']') {
                delimiters = {header.substr(1, header.size() - 2)};
            } else if (!header.empty()) {
                delimiters = {header};
            }

            numbersPart = input.substr(newlinePos + 1);
            return delimiters;
        }
    }

    numbersPart = input;
    return delimiters;
}

std::vector<int> StringCalculator::toNumbers(const std::vector<std::string>& tokens) {
    std::vector<int> nums;
    for (const auto& t : tokens) {
        if (!t.empty()) nums.push_back(std::stoi(t));
    }
    return nums;
}
