#include "string_calculator.h"
#include <sstream>
#include <regex>
#include <algorithm>

// --- Public method ---
int StringCalculator::add(const std::string& input) {
    if (input.empty()) return 0;

    const bool custom = hasCustomDelimiter(input);
    auto [header, numbersStr] = custom
        ? splitHeaderAndNumbers(input)
        : std::make_pair(std::string(), input);

    // Always include default delimiters; add custom ones if present
    std::vector<std::string> delimiters{",", "\n"};
    if (!header.empty()) {
        auto customDelims = extractDelimiters(header);
        delimiters.insert(delimiters.end(), customDelims.begin(), customDelims.end());
    }

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
    // input starts with "//"
    auto pos = input.find('\n');
    if (pos == std::string::npos) {
        // Malformed header: treat as "no numbers" to yield 0 per clarification
        return {input.substr(2), ""};
    }
    return {input.substr(2, pos - 2), input.substr(pos + 1)};
}

std::vector<std::string>
StringCalculator::extractDelimiters(const std::string& header) const {
    std::vector<std::string> delimiters;
    if (header.rfind("[", 0) == 0) {
        // Bracketed multi-char delimiters: collect all [..]
        std::regex rx("\\[(.*?)\\]");
        std::sregex_iterator it(header.begin(), header.end(), rx), end;
        for (; it != end; ++it) {
            delimiters.push_back(it->str(1));
        }
        // If no captures (e.g., "//[]"), treat as empty custom -> ignore, defaults already included
    } else if (!header.empty()) {
        // Single-character delimiter (unbracketed)
        delimiters.push_back(header.substr(0, 1));
    }
    return delimiters;
}

static std::string escapeRegex(const std::string& s) {
    // Escape regex metacharacters
    static const std::regex special(R"([\\^$.[|()?*+{}])");
    return std::regex_replace(s, special, R"(\\$&)");
}

std::vector<std::string>
StringCalculator::tokenize(const std::string& numbersStr,
                           const std::vector<std::string>& delimiters) const {
    if (numbersStr.empty()) return {};

    // Build alternation of all delimiters
    std::string pattern;
    for (size_t i = 0; i < delimiters.size(); ++i) {
        pattern += escapeRegex(delimiters[i]);
        if (i + 1 < delimiters.size()) pattern += "|";
    }

    std::regex splitter(pattern);
    std::sregex_token_iterator it(numbersStr.begin(), numbersStr.end(), splitter, -1), end;
    std::vector<std::string> out{it, end};

    // Keep empty tokens; parseNumbers filters non-numeric safely
    return out;
}

std::vector<int>
StringCalculator::parseNumbers(const std::vector<std::string>& tokens) const {
    std::vector<int> numbers;
    static const std::regex intRx(R"(^-?\d+$)");
    for (const auto& token : tokens) {
        if (token.empty()) continue;              // skip empty
        if (!std::regex_match(token, intRx)) continue; // skip non-integer tokens
        try {
            numbers.push_back(std::stoi(token));
        } catch (...) {
            // Safety net: skip non-convertible tokens
        }
    }
    return numbers;
}

void StringCalculator::validateNoNegatives(const std::vector<int>& numbers) const {
    std::vector<int> negatives;
    for (int n : numbers) {
        if (n < 0) negatives.push_back(n);
    }
    if (!negatives.empty()) {
        std::ostringstream oss;
        oss << "negatives not allowed: ";
        for (size_t i = 0; i < negatives.size(); ++i) {
            if (i) oss << ", ";
            oss << negatives[i];
        }
        throw NegativeNumberException(oss.str());
    }
}
