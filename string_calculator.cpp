#include "string_calculator.h"
#include <sstream>
#include <regex>
#include <algorithm>

// --- Public method ---
int StringCalculator::add(const std::string& input) {
    if (input.empty()) return 0;

    auto [header, numbersStr] = hasCustomDelimiter(input)
                                    ? splitHeaderAndNumbers(input)
                                    : std::make_pair(std::string(), input);

    auto delimiters = header.empty()
                        ? std::vector<std::string>{",", "\n"}
                        : extractDelimiters(header);

    auto tokens = tokenize(numbersStr, delimiters);
    auto numbers = parseNumbers(tokens);

    validateNoNegatives(numbers);

    int sum = 0;
    for (int n : numbers) {
        if (n <= 1000) sum += n;
    }
    return sum;
}

// --- Private helpers ---

bool StringCalculator::hasCustomDelimiter(const std::string& input) const {
    return input.rfind("//", 0) == 0;
}

std::pair<std::string, std::string>
StringCalculator::splitHeaderAndNumbers(const std::string& input) const {
    auto pos = input.find('\n');
    if (pos == std::string::npos) {
        return {input, ""}; // malformed, spec allows ignoring
    }
    return {input.substr(2, pos - 2), input.substr(pos + 1)};
}

std::vector<std::string>
StringCalculator::extractDelimiters(const std::string& header) const {
    std::vector<std::string> delimiters;
    if (header.rfind("[", 0) == 0) {
        std::regex rx("\\[(.*?)\\]");
        std::sregex_iterator it(header.begin(), header.end(), rx), end;
        for (; it != end; ++it) {
            delimiters.push_back(it->str(1));
        }
    } else {
        delimiters.push_back(header.substr(0, 1));
    }
    if (delimiters.empty()) {
        delimiters = {",", "\n"};
    }
    return delimiters;
}

std::vector<std::string>
StringCalculator::tokenize(const std::string& numbersStr,
                           const std::vector<std::string>& delimiters) const {
    if (numbersStr.empty()) return {};

    std::string regexPattern;
    for (size_t i = 0; i < delimiters.size(); ++i) {
        regexPattern += std::regex_replace(delimiters[i], std::regex(R"([\\^$.[|()?*+{}])"), R"(\\$&)");
        if (i != delimiters.size() - 1) regexPattern += "|";
    }

    std::regex splitter(regexPattern);
    std::sregex_token_iterator it(numbersStr.begin(), numbersStr.end(), splitter, -1), end;
    return {it, end};
}

std::vector<int>
StringCalculator::parseNumbers(const std::vector<std::string>& tokens) const {
    std::vector<int> numbers;
    for (const auto& token : tokens) {
        if (!token.empty()) {
            numbers.push_back(std::stoi(token));
        }
    }
    return numbers;
}

void StringCalculator::validateNoNegatives(const std::vector<int>& numbers) const {
    std::vector<int> negatives;
    for (int n : numbers) if (n < 0) negatives.push_back(n);

    if (!negatives.empty()) {
        std::ostringstream oss;
        oss << "negatives not allowed: ";
        for (size_t i = 0; i < negatives.size(); ++i) {
            oss << negatives[i];
            if (i < negatives.size() - 1) oss << ", ";
        }
        throw NegativeNumberException(oss.str());
    }
}
