#include "string_calculator.h"
#include <regex>
#include <sstream>
#include <algorithm>

// ---- Tiny utilities (each CCN = 1) ----
namespace {
    inline bool startsWith(const std::string& s, const char* pfx) {
        return s.rfind(pfx, 0) == 0;
    }
    inline std::string escapeRegex(const std::string& s) {
        static const std::regex special(R"([\\^$.[|()?*+{}])");
        return std::regex_replace(s, special, R"(\\$&)");
    }
    inline bool isIntegerToken(const std::string& t) {
        static const std::regex intRx(R"(^-?\d+$)");
        return std::regex_match(t, intRx);
    }
    inline std::string joinDelimsAsRegex(const std::vector<std::string>& delims) {
        std::string pattern;
        for (size_t i = 0; i < delims.size(); ++i) {
            pattern += escapeRegex(delims[i]);
            if (i + 1 < delims.size()) pattern += "|";
        }
        return pattern;
    }
}

// ---- Public API (CCN ≤ 3) ----
int StringCalculator::add(const std::string& input) {
    if (input.empty()) return 0;

    const bool headerPresent = hasCustomDelimiter(input);
    auto parts = headerPresent
        ? splitHeaderAndNumbers(input)
        : std::make_pair(std::string(), input);

    // Always include defaults, then add valid custom delimiters (if any)
    std::vector<std::string> delimiters{",", "\n"};
    if (!parts.first.empty()) {
        auto custom = extractDelimiters(parts.first);
        delimiters.insert(delimiters.end(), custom.begin(), custom.end());
    }

    auto tokens = tokenize(parts.second, delimiters);
    auto numbers = parseNumbers(tokens);
    validateNoNegatives(numbers);

    int sum = 0;
    for (int n : numbers) if (n <= 1000) sum += n;
    return sum;
}

// ---- Private helpers ----

// CCN = 1
bool StringCalculator::hasCustomDelimiter(const std::string& input) const {
    return startsWith(input, "//");
}

// CCN = 2
std::pair<std::string, std::string>
StringCalculator::splitHeaderAndNumbers(const std::string& input) const {
    auto pos = input.find('\n');
    if (pos == std::string::npos) {
        // Malformed header: undefined → produce empty numbers to yield 0
        return {input.substr(2), ""};
    }
    return {input.substr(2, pos - 2), input.substr(pos + 1)};
}

// CCN = 3
std::vector<std::string>
StringCalculator::extractDelimiters(const std::string& header) const {
    std::vector<std::string> delims;

    if (startsWith(header, "[")) {
        // Collect all non-empty [..]
        std::regex rx(R"(\[(.*?)\])");
        for (std::sregex_iterator it(header.begin(), header.end(), rx), end; it != end; ++it) {
            const auto d = (*it).str(1);
            if (!d.empty()) delims.push_back(d);
        }
        return delims; // may be empty -> defaults still apply
    }

    // Unbracketed: only single character is valid
    if (header.size() == 1) delims.push_back(header);
    return delims;
}

// CCN = 2
std::vector<std::string>
StringCalculator::tokenize(const std::string& numbersStr,
                           const std::vector<std::string>& delimiters) const {
    if (numbersStr.empty()) return {};
    std::regex splitter(joinDelimsAsRegex(delimiters));
    std::sregex_token_iterator it(numbersStr.begin(), numbersStr.end(), splitter, -1), end;
    return {it, end}; // may contain empties; parsed safely later
}

// CCN = 3
std::vector<int>
StringCalculator::parseNumbers(const std::vector<std::string>& tokens) const {
    std::vector<int> nums;
    for (const auto& t : tokens) {
        if (t.empty()) continue;
        if (!isIntegerToken(t)) continue;
        try { nums.push_back(std::stoi(t)); } catch (...) { /* skip */ }
    }
    return nums;
}

// CCN = 3
void StringCalculator::validateNoNegatives(const std::vector<int>& numbers) const {
    std::vector<int> neg;
    for (int n : numbers) if (n < 0) neg.push_back(n);
    if (neg.empty()) return;

    std::ostringstream oss;
    oss << "negatives not allowed: ";
    for (size_t i = 0; i < neg.size(); ++i) {
        if (i) oss << ", ";
        oss << neg[i];
    }
    throw NegativeNumberException(oss.str());
}
