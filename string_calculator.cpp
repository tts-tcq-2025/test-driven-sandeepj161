#include "./string_calculator.h"

#include <charconv>
#include <regex>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace {
inline bool startsWith(const std::string& s, const char* pfx) {
  return s.rfind(pfx, 0) == 0;
}

inline std::string escapeRegex(const std::string& s) {
  static const std::regex kSpecial(R"([\\^$.[|()?*+{}])");
  return std::regex_replace(s, kSpecial, R"(\\$&)");
}

inline bool isIntegerToken(const std::string& t) {
  static const std::regex kIntRx(R"(^-?\d+$)");
  return std::regex_match(t, kIntRx);
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

inline std::pair<bool, int> toIntNoThrow(const std::string& s) {
  int value = 0;
  const char* begin = s.data();
  const char* end = s.data() + s.size();
  auto res = std::from_chars(begin, end, value);
  if (res.ec != std::errc{} || res.ptr != end) return {false, 0};
  return {true, value};
}
}  // namespace

// Keep add linear and short (CCN <= 3): early return, no nested branches.
int StringCalculator::add(const std::string& input) {
  if (input.empty()) return 0;

  std::string header;
  std::string payload;

  if (hasCustomDelimiter(input)) {
    auto hp = splitHeaderAndNumbers(input);
    header = std::move(hp.first);
    payload = std::move(hp.second);
  } else {
    payload = input;
  }

  std::vector<std::string> delimiters{",", "\n"};
  if (!header.empty()) {
    auto extra = extractDelimiters(header);
    delimiters.insert(delimiters.end(), extra.begin(), extra.end());
  }

  auto tokens = tokenize(payload, delimiters);
  auto numbers = parseNumbers(tokens);
  validateNoNegatives(numbers);

  int sum = 0;
  for (int n : numbers) if (n <= 1000) sum += n;
  return sum;
}

// Straight string prefix check; keeps CCN minimal.
bool StringCalculator::hasCustomDelimiter(const std::string& input) const {
  return input.size() >= 2 && input[0] == '/' && input[1] == '/';
}

// Simple split; malformed header => empty numbers to yield 0.
std::pair<std::string, std::string> StringCalculator::splitHeaderAndNumbers(
    const std::string& input) const {
  const auto pos = input.find('\n');
  if (pos == std::string::npos) return {input.substr(2), ""};
  return {input.substr(2, pos - 2), input.substr(pos + 1)};
}

// Helpers for delimiter extraction to keep complexity low.
static std::vector<std::string> extractBracketedDelims(const std::string& header) {
  std::vector<std::string> delims;
  std::regex rx(R"(\[(.*?)\])");
  for (std::sregex_iterator it(header.begin(), header.end(), rx), end; it != end; ++it) {
    const auto d = (*it).str(1);
    if (!d.empty()) delims.push_back(d);
  }
  return delims;  // may be empty => defaults only
}
static std::vector<std::string> extractSingleCharDelim(const std::string& header) {
  if (header.size() == 1) return {header};
  return {};
}

// Flat dispatch: bracketed multi-char or exactly one char, else none.
std::vector<std::string> StringCalculator::extractDelimiters(
    const std::string& header) const {
  if (!header.empty() && header.front() == '[') return extractBracketedDelims(header);
  return extractSingleCharDelim(header);
}

// Tokenize is linear and pure.
std::vector<std::string> StringCalculator::tokenize(
    const std::string& payload, const std::vector<std::string>& delimiters) const {
  if (payload.empty()) return {};
  std::regex splitter(joinAlternation(delimiters));
  std::sregex_token_iterator it(payload.begin(), payload.end(), splitter, -1), end;
  return {it, end};
}

// Single loop with guard-continues; from_chars avoids exceptions (CCN <= 3).
std::vector<int> StringCalculator::parseNumbers(
    const std::vector<std::string>& tokens) const {
  std::vector<int> nums;
  nums.reserve(tokens.size());
  for (const auto& t : tokens) {
    if (t.empty()) continue;
    if (!isIntegerToken(t)) continue;
    auto parsed = toIntNoThrow(t);
    if (parsed.first) nums.push_back(parsed.second);
  }
  return nums;
}

// Linear check and early return (CCN <= 3).
void StringCalculator::validateNoNegatives(const std::vector<int>& numbers) const {
  std::vector<int> negatives;
  for (int n : numbers) if (n < 0) negatives.push_back(n);
  if (negatives.empty()) return;

  std::ostringstream oss;
  oss << "negatives not allowed: ";
  for (size_t i = 0; i < negatives.size(); ++i) {
    if (i) oss << ", ";
    oss << negatives[i];
  }
  throw NegativeNumberException(oss.str());
}
