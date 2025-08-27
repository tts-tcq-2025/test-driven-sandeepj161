#include "string_calculator.h"
#include <regex>
#include <sstream>
#include <algorithm>

// Small helpers (keep CCN low)
namespace {
    bool startsWith(const std::string& s, const char* pfx) {
        return s.rfind(pfx, 0) == 0;
    }
    std::string escapeRegex(const std::string& s) {
        static const std::regex special(R"([\\^$.[|()?*+{}])");
        return std::regex_replace(s, special, R"(\\$&)");
    }
    bool isIntegerToken(const std::string& t) {
        static const std::regex intRx(R"(^-?\d+$)");
        return std::regex_match(t, intRx);
    }
    std::string joinDelimsAsRegex(const std::vector<std::string>& delims) {
        std::string pattern;
        for (size_t i = 0; i < delims.size(); ++i) {
            pattern += escapeRegex(delims[i]);
            if (i + 1 < delims.size()) pattern += "|";
        }
        return pattern;
    }
}

// Public API (CCN <= 3)
int StringCalculator::add(const std::string& input) {
    if (input.empty()) return 0;

    const bool hasHeader = hasCustomDelimiter(input);
    auto [header, numbersStr] = hasHeader
        ? splitHeaderAndNumbers(input)
        : std::make_pair(std::string(), input);

    // Always include defaults; add valid custom delimiters if present
    std::vector<std::string> delimiters{",", "\n"};
    if (!header.empty()) {
        auto custom = extractDelimiters(header);
        delimiters.insert(delimiters.end(), custom.begin(), custom.end());
    }

    auto tokens = tokenize(numbersStr, delimiters);
    auto numbers = parseNumbers(tokens);
    validateNoNegatives(numbers);

    int sum = 0;
    for (int n : numbers) if (n <= 1000) sum += n;
    return sum;
}

// CCN = 1
bool StringCalculator::hasCustomDelimiter(const std::string& input) const {
    return startsWith(input, "//");
}

// CCN = 2
std::pair<std::string, std::string>
StringCalculator::splitHeaderAndNumbers(const std::string& input) const {
    auto pos = input.find('\n');
    if (pos == std::string::npos) {
        // Malformed header: undefined by spec → return zero by providing empty numbers
        return {input.substr(2), ""};
    }
    // strip leading "//"
    return {input.substr(2, pos - 2), input.substr(pos + 1)};
}

// CCN = 3 (handles three branches)
std::vector<std::string>
StringCalculator::extractDelimiters(const std::string& header) const {
    std::vector<std::string> delims;

    if (startsWith(header, "[")) {
        // Collect all [..] as multi-char delimiters; empty [] means ignore
        std::regex rx(R"(\[(.*?)\])");
        for (std::sregex_iterator it(header.begin(), header.end(), rx), end; it != end; ++it) {
            const auto d = (*it).str(1);
            if (!d.empty()) delims.push_back(d);
        }
        return delims; // may be empty → defaults still apply
    }

    // Unbracketed: accept only single character delimiter
    if (header.size() == 1) delims.push_back(header);
    return delims; // if length > 1 (e.g., "***"), ignore → defaults only
}

// CCN = 3
std::vector<std::string>
StringCalculator::tokenize(const std::string& numbersStr,
                           const std::vector<std::string>& delimiters) const {
    if (numbersStr.empty()) return {};
    std::regex splitter(joinDelimsAsRegex(delimiters));
    std::sregex_token_iterator it(numbersStr.begin(), numbersStr.end(), splitter, -1), end;
    return {it, end}; // keep empties; parseNumbers will skip
}

// CCN = 3
std::vector<int>
StringCalculator::parseNumbers(const std::vector<std::string>& tokens) const {
    std::vector<int> nums;
    for (const auto& t : tokens) {
        if (t.empty()) continue;
        if (!isIntegerToken(t)) continue; // skip non-numeric tokens
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
