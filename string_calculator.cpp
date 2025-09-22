#include "string_calculator.h"
#include <sstream>
#include <cctype>
#include <algorithm>

NegativeNumberException::NegativeNumberException(const std::vector<int>& negatives)
    : std::runtime_error("Negatives not allowed"), negatives_(negatives) {}

const std::vector<int>& NegativeNumberException::getNegatives() const {
    return negatives_;
}

int StringCalculator::add(const std::string& input) {
    if (input.empty()) return 0;

    bool hasHeader = false;
    size_t numbersStart = 0;
    std::string header = extractHeader(input, hasHeader, numbersStart);

    std::vector<std::string> delimiters = {",", "\n"};
    if (hasHeader) {
        bool valid = true;
        auto customDelims = extractDelimiters(header, valid);
        if (valid && !customDelims.empty()) {
            delimiters.insert(delimiters.end(), customDelims.begin(), customDelims.end());
        } else if (!valid) {
            return 0; // invalid header → return 0
        }
    }

    std::string numbersPart = input.substr(numbersStart);
    auto tokens = split(numbersPart, delimiters);
    return sumNumbers(tokens);
}

// --- Parsing helpers ---

std::string StringCalculator::extractHeader(const std::string& input, bool& hasHeader, size_t& numbersStart) {
    hasHeader = false;
    numbersStart = 0;

    if (input.rfind("//", 0) == 0) {
        size_t newlinePos = input.find('\n');
        if (newlinePos == std::string::npos) return ""; // no newline → invalid header
        hasHeader = true;
        numbersStart = newlinePos + 1;
        return input.substr(2, newlinePos - 2);
    }
    return "";
}

std::vector<std::string> StringCalculator::extractDelimiters(const std::string& header, bool& valid) {
    std::vector<std::string> delimiters;
    valid = true;

    if (header.empty()) return delimiters;

    if (header[0] == '[') {
        // Bracketed delimiters
        size_t pos = 0;
        while (pos < header.size()) {
            if (header[pos] != '[') { valid = false; break; }
            size_t end = header.find(']', pos);
            if (end == std::string::npos) { valid = false; break; }
            std::string delim = header.substr(pos + 1, end - pos - 1);
            if (!delim.empty()) delimiters.push_back(delim);
            pos = end + 1;
        }
    } else {
        // Single char delimiter only
        if (header.size() == 1) {
            delimiters.push_back(header);
        } else {
            valid = false; // multi-char without brackets → invalid
        }
    }
    return delimiters;
}

std::vector<std::string> StringCalculator::split(const std::string& str, const std::vector<std::string>& delimiters) {
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t pos = 0;

    while (pos < str.size()) {
        bool matched = false;
        for (const auto& delim : delimiters) {
            if (!delim.empty() && str.compare(pos, delim.size(), delim) == 0) {
                tokens.push_back(str.substr(start, pos - start));
                pos += delim.size();
                start = pos;
                matched = true;
                break;
            }
        }
        if (!matched) {
            pos++;
        }
    }
    tokens.push_back(str.substr(start));
    return tokens;
}

// --- Number handling ---

bool StringCalculator::isNumber(const std::string& token) {
    if (token.empty()) return false;
    if (token == "-" || token == "+") return false;
    return std::all_of(token.begin(), token.end(), [](char c) { return std::isdigit(c) || c == '-' || c == '+'; });
}

int StringCalculator::sumNumbers(const std::vector<std::string>& tokens) {
    int sum = 0;
    std::vector<int> negatives;

    for (const auto& token : tokens) {
        if (!isNumber(token)) continue;

        int num = std::stoi(token);
        if (num < 0) negatives.push_back(num);
        else if (num <= 1000) sum += num;
    }

    if (!negatives.empty()) throw NegativeNumberException(negatives);
    return sum;
}
