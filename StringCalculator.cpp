#include "StringCalculator.h"

#include <sstream>
#include <regex>
#include <algorithm>

// ---------------- NegativeNumberException ----------------
NegativeNumberException::NegativeNumberException(const std::vector<int>& negatives)
    : std::runtime_error(buildMessage(negatives)) {}

std::string NegativeNumberException::buildMessage(const std::vector<int>& negatives) {
    std::ostringstream oss;
    oss << "negatives not allowed: ";
    for (size_t i = 0; i < negatives.size(); ++i) {
        oss << negatives[i];
        if (i + 1 < negatives.size()) oss << ", ";
    }
    return oss.str();
}

// ---------------- StringCalculator ----------------
int StringCalculator::add(const std::string& input) {
    if (input.empty()) return 0;

    auto [delimiters, numbersStr] = extractDelimiter(input);
    auto tokens = splitNumbers(numbersStr, delimiters);
    auto numbers = validateNumbers(tokens);
    return sumNumbers(numbers);
}

StringCalculator::DelimiterResult StringCalculator::extractDelimiter(const std::string& input) {
    if (input.rfind("//", 0) == 0) { // header present at very beginning
        size_t newlinePos = input.find('\n');
        if (newlinePos == std::string::npos) {
            // malformed header — spec says this is undefined; choose sensible fallback:
            // treat as no numbers line (i.e., 0)
            return {defaultDelimiters(), ""};
        }
        std::string header = input.substr(2, newlinePos - 2);
        std::string numbersPart = input.substr(newlinePos + 1);
        return {parseCustomDelimiters(header), numbersPart};
    }
    return {defaultDelimiters(), input};
}

std::vector<std::string> StringCalculator::parseCustomDelimiters(const std::string& header) {
    std::vector<std::string> delimiters;
    if (header.empty()) {
        return defaultDelimiters();
    }

    if (header.front() == '[' && header.back() == ']') {
        // capture every [...] occurrence: supports multiple bracketed delimiters
        std::regex re(R"(\[([^\]]*)\])");
        auto begin = std::sregex_iterator(header.begin(), header.end(), re);
        auto end = std::sregex_iterator();
        for (auto it = begin; it != end; ++it) {
            std::string d = (*it)[1].str();
            if (!d.empty()) delimiters.push_back(d);
        }
    } else {
        // single-char custom delimiter (take only first character)
        delimiters.push_back(header.substr(0, 1));
    }

    // If parsing produced no useful delimiters (e.g. //[]), fall back to defaults.
    if (delimiters.empty()) return defaultDelimiters();
    return delimiters;
}

std::vector<std::string> StringCalculator::splitNumbers(
    const std::string& input, const std::vector<std::string>& delimiters) 
{
    if (input.empty()) return {};

    // Build an "alternation" regex pattern of the delimiters (escaped).
    // We escape regex-special chars in each delimiter.
    auto escapeForRegex = [](const std::string& s) {
        static const std::regex special(R"([\^\$\.\|\?\*\+\(\)\[\]\{\}\\])");
        return std::regex_replace(s, special, R"(\\$&)");
    };

    std::string pattern;
    for (size_t i = 0; i < delimiters.size(); ++i) {
        pattern += escapeForRegex(delimiters[i]);
        if (i + 1 < delimiters.size()) pattern += "|";
    }

    // Use regex token iterator to split (tokens are parts between delimiters).
    std::regex re(pattern);
    std::sregex_token_iterator it(input.begin(), input.end(), re, -1);
    std::sregex_token_iterator end;
    return {it, end};
}

std::vector<int> StringCalculator::validateNumbers(const std::vector<std::string>& tokens) {
    std::vector<int> numbers;
    std::vector<int> negatives;

    for (const auto& token : tokens) {
        if (token.empty()) continue; // skip empty tokens gracefully
        // Trim whitespace around token (robustness).
        size_t start = 0;
        while (start < token.size() && isspace(static_cast<unsigned char>(token[start]))) ++start;
        size_t end = token.size();
        while (end > start && isspace(static_cast<unsigned char>(token[end - 1]))) --end;
        if (start >= end) continue;

        std::string trimmed = token.substr(start, end - start);

        // stoi may throw if token isn't a number (this surfaces to test when input malformed).
        int num = std::stoi(trimmed);
        if (num < 0) negatives.push_back(num);
        else if (num <= MAX_NUMBER) numbers.push_back(num);
        // numbers > MAX_NUMBER are ignored
    }

    if (!negatives.empty()) {
        throw NegativeNumberException(negatives);
    }
    return numbers;
}

int StringCalculator::sumNumbers(const std::vector<int>& numbers) {
    int sum = 0;
    for (int n : numbers) sum += n;
    return sum;
}

std::vector<std::string> StringCalculator::defaultDelimiters() {
    return {",", "\n"};
}
