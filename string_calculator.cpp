#include "./string_calculator.h"
#include <sstream>
#include <regex>
#include <algorithm>

int StringCalculator::add(const std::string& input) {
    if (input.empty()) return 0;

    auto [delimiters, numbersPart] = parseHeader(input);
    std::vector<int> numbers = tokenize(numbersPart, delimiters);
    return computeSum(numbers);
}

// Parse optional header (e.g. //;\n or //[***]\n)
std::pair<std::vector<std::string>, std::string>
StringCalculator::parseHeader(const std::string& input) const {
    if (input.rfind("//", 0) != 0) {
        return {{",", "\n"}, input};
    }

    size_t newlinePos = input.find('\n');
    if (newlinePos == std::string::npos) {
        return {{",", "\n"}, input};  // fallback if malformed
    }

    std::string header = input.substr(2, newlinePos - 2);
    std::vector<std::string> delimiters;

    if (header.size() >= 2 && header.front() == '[' && header.back() == ']') {
        std::regex re("\\[(.*?)\\]");
        std::sregex_iterator it(header.begin(), header.end(), re), end;
        while (it != end) {
            delimiters.push_back((*it)[1]);
            ++it;
        }
    } else {
        delimiters.push_back(header);
    }

    if (delimiters.empty()) delimiters = {",", "\n"};
    return {delimiters, input.substr(newlinePos + 1)};
}

// Split by any delimiter, convert to ints, enforce rules
std::vector<int> StringCalculator::tokenize(
    const std::string& input,
    const std::vector<std::string>& delimiters) const {

    // Build regex like (delim1|delim2|...)
    std::string pattern;
    for (size_t i = 0; i < delimiters.size(); i++) {
        if (i) pattern += "|";
        pattern += "(" + std::regex_replace(delimiters[i], std::regex(R"([\^\$\.\|\?\*\+\(\)\[\{\\])"), R"(\\$&)") + ")";
    }
    std::regex re(pattern);

    std::sregex_token_iterator it(input.begin(), input.end(), re, -1), end;
    std::vector<int> numbers;
    for (; it != end; ++it) {
        if (it->str().empty()) continue;
        int value = std::stoi(it->str());
        numbers.push_back(value);
    }
    return numbers;
}

// Sum numbers, throw on negatives, ignore >1000
int StringCalculator::computeSum(const std::vector<int>& numbers) const {
    std::vector<int> negatives;
    int sum = 0;

    for (int n : numbers) {
        if (n < 0) negatives.push_back(n);
        else if (n <= 1000) sum += n;
    }

    if (!negatives.empty()) {
        std::ostringstream oss;
        oss << "negatives not allowed: ";
        for (size_t i = 0; i < negatives.size(); i++) {
            if (i) oss << ", ";
            oss << negatives[i];
        }
        throw NegativeNumberException(oss.str());
    }

    return sum;
}
