#include "string_calculator.h"
#include <regex>
#include <sstream>
#include <charconv>
#include <algorithm>

// ---------- Tiny utilities (all pure, CCN = 1) ----------
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
    inline std::string joinAlternation(const std::vector<std::string>& delims) {
        std::string pattern;
        pattern.reserve(delims.size() * 3);
        for (size_t i = 0; i < delims.size(); ++i) {
            pattern += escapeRegex(delims[i]);
            if (i + 1 < delims.size()) pattern += "|";
        }
        return pattern;
    }
    inline std::pair<bool,int> toIntNoThrow(const std::string& s) {
        int value = 0;
        auto* begin = s.data();
        auto* end   = s.data() + s.size();
        auto res = std::from_chars(begin, end, value);
        if (res.ec != std::errc{} || res.ptr != end) return {false, 0};
        return {true, value};
    }
    inline bool isBracketedHeader(const std::string& header) {
        return !header.empty() && header.front() == '[';
    }
    inline bool isSingleCharHeader(const std::string& header) {
        return header.size() == 1;
    }
}

// ---------- Public API (CCN ≤ 3) ----------
int StringCalculator::add(const std::string& input) {
    if (input.empty()) return 0;

    std::string header;
    std::string payload;

    if (hasCustomDelimiter(input)) {
        auto p = splitHeaderAndNumbers(input);
        header  = std::move(p.first);
        payload = std::move(p.second);
    } else {
        payload = input;
    }

    // Defaults always active; custom augment them
    std::vector<std::string> delimiters{",", "\n"};
    if (!header.empty()) {
        auto custom = extractDelimiters(header);
        delimiters.insert(delimiters.end(), custom.begin(), custom.end());
    }

    auto tokens  = tokenize(payload, delimiters);
    auto numbers = parseNumbers(tokens);

    validateNoNegatives(numbers);

    int sum = 0;
    for (int n : numbers) if (n <= 1000) sum += n;
    return sum;
}

// ---------- Header parsing ----------
bool StringCalculator::hasCustomDelimiter(const std::string& input) const {
    return startsWith(input, "//");
}

std::pair<std::string, std::string>
StringCalculator::splitHeaderAndNumbers(const std::string& input) const {
    auto pos = input.find('\n');
    if (pos == std::string::npos) {
        // Malformed header => undefined by spec; choose 0 by providing no numbers
        return {input.substr(2), ""};
    }
    // Strip leading "//"
    return {input.substr(2, pos - 2), input.substr(pos + 1)};
}

// ---------- Delimiter extraction (extensible) ----------
static std::vector<std::string> extractBracketedDelims(const std::string& header) {
    std::vector<std::string> delims;
    std::regex rx(R"(\[(.*?)\])");
    for (std::sregex_iterator it(header.begin(), header.end(), rx), end; it != end; ++it) {
        const auto d = (*it).str(1);
        if (!d.empty()) delims.push_back(d);
    }
    return delims; // may be empty => defaults only
}

static std::vector<std::string> extractSingleCharDelim(const std::string& header) {
    if (header.size() == 1) return {header};
    return {}; // ignore invalid unbracketed multi-char like "***"
}

std::vector<std::string>
StringCalculator::extractDelimiters(const std::string& header) const {
    if (isBracketedHeader(header)) return extractBracketedDelims(header);
    return extractSingleCharDelim(header);
}

// ---------- Tokenization and parsing ----------
std::vector<std::string>
StringCalculator::tokenize(const std::string& payload,
                           const std::vector<std::string>& delimiters) const {
    if (payload.empty()) return {};
    std::regex splitter(joinAlternation(delimiters));
    std::sregex_token_iterator it(payload.begin(), payload.end(), splitter, -1), end;
    return {it, end}; // empties allowed; filtered in parseNumbers
}

std::vector<int>
StringCalculator::parseNumbers(const std::vector<std::string>& tokens) const {
    std::vector<int> nums;
    nums.reserve(tokens.size());
    for (const auto& t : tokens) {
        if (t.empty()) continue;
        if (!isIntegerToken(t)) continue;
        auto [ok, v] = toIntNoThrow(t);
        if (ok) nums.push_back(v);
    }
    return nums;
}

// ---------- Rules ----------
void StringCalculator::validateNoNegatives(const std::vector<int>& numbers) const {
    std::vector<int> negs;
    for (int n : numbers) if (n < 0) negs.push_back(n);
    if (negs.empty()) return;

    std::ostringstream oss;
    oss << "negatives not allowed: ";
    for (size_t i = 0; i < negs.size(); ++i) {
        if (i) oss << ", ";
        oss << negs[i];
    }
    throw NegativeNumberException(oss.str());
}
