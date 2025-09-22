#include "StringCalculator.h"
#include <regex>
#include <sstream>
#include <stdexcept>
#include <algorithm>

int StringCalculator::add(const std::string& input) {
    if (isBlank(input)) return 0;
    Parsed parsed = parseInput(input);
    std::vector<int> numbers = splitNumbers(parsed.numbers, parsed.delimiters);
    validateNoNegatives(numbers);
    return calculateSum(numbers);
}

// ---------------- Helper Methods ----------------

bool StringCalculator::isBlank(const std::string& input) {
    return input.empty();
}

StringCalculator::Parsed StringCalculator::parseInput(const std::string& input) {
    const std::vector<std::string> DEFAULT_DELIMITERS = {",", "\n"};

    if (input.rfind("//", 0) == 0) {
        size_t idx = input.find('\n');
        std::vector<std::string> delimiters = DEFAULT_DELIMITERS;
        auto customDelims = parseCustomDelimiters(input.substr(2, idx - 2));
        delimiters.insert(delimiters.end(), customDelims.begin(), customDelims.end());
        std::string numbers = input.substr(idx + 1);
        return Parsed{numbers, delimiters};
    }
    return Parsed{input, DEFAULT_DELIMITERS};
}

std::vector<std::string> StringCalculator::parseCustomDelimiters(const std::string& part) {
    std::vector<std::string> result;

    if (!part.empty() && part.front() == '[' && part.back() == ']') {
        std::regex re(R"(\[(.*?)\])");
        auto begin = std::sregex_iterator(part.begin(), part.end(), re);
        auto end = std::sregex_iterator();

        for (auto i = begin; i != end; ++i) {
            result.push_back((*i)[1].str());
        }
    } else if (!part.empty()) {
        result.push_back(part);
    }

    return result;
}

std::vector<int> StringCalculator::splitNumbers(const std::string& numbers, const std::vector<std::string>& delimiters) {
    std::string regexPattern;
    for (size_t i = 0; i < delimiters.size(); ++i) {
        if (i != 0) regexPattern += "|";
        std::string d = std::regex_replace(delimiters[i], std::regex(R"([\^$.|?*+(){}\[\]\\])"), R"(\\$&)");
        regexPattern += d;
    }

    std::regex re(regexPattern);
    std::sregex_token_iterator it(numbers.begin(), numbers.end(), re, -1);
    std::sregex_token_iterator end;

    std::vector<int> result;
    for (; it != end; ++it) {
        if (!it->str().empty()) result.push_back(std::stoi(it->str()));
    }
    return result;
}

void StringCalculator::validateNoNegatives(const std::vector<int>& numbers) {
    std::vector<int> negatives;
    for (int n : numbers) if (n < 0) negatives.push_back(n);

    if (!negatives.empty()) {
        std::string msg = "negatives not allowed: ";
        for (size_t i = 0; i < negatives.size(); ++i) {
            msg += std::to_string(negatives[i]);
            if (i < negatives.size() - 1) msg += ", ";
        }
        throw std::runtime_error(msg);
    }
}

int StringCalculator::calculateSum(const std::vector<int>& numbers) {
    int sum = 0;
    for (int n : numbers) if (n <= 1000) sum += n;
    return sum;
}
