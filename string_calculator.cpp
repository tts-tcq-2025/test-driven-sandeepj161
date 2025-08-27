#include "./string_calculator.h"

#include <algorithm>  // std::move
#include <charconv>   // std::from_chars
#include <regex>      // std::regex, std::sregex_iterator, std::regex_match
#include <sstream>    // std::ostringstream
#include <string>     // std::string
#include <utility>    // std::pair
#include <vector>     // std::vector

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

inline bool isBracketedHeader(const std::string& header) {
  return !header.empty() && header.front() == '[';
}
}  // namespace

int StringCalculator::add(const std::string& input) {
  // Minimal requirement from provided Python test: empty input => 0
  if (input.empty()) return 0;

  // The following is a future-ready structure; currently kept minimal.
  // Default delimiters
  std::vector<std::string> delimiters{",", "\n"};

  // If custom delimiter exists, split off the header and extend delimiters.
  std::string header;
  std::string payload;

  if (hasCustomDelimiter(input)) {
    auto split = splitHeaderAndNumbers(input);
    header = std::move(split.first);
    payload = std::move(split.second);
    if (!header.empty()) {
      auto custom = extractDelimiters(header);
      delimiters.insert(delimiters.end(), custom.begin(), custom.end());
    }
  } else {
    payload = input;
  }

  // For the current minimal requirement, allow single number or empty only.
  auto tokens = tokenize(payload, delimiters);
  auto numbers = parseNumbers(tokens);

  validateNoNegatives(numbers);

  int sum = 0;
  for (int n : numbers) {
    if (n <= 1000) sum += n;
  }
  return sum;
}

bool StringCalculator::hasCustomDelimiter(const std::string& input) const {
  return input.size() >= 2 && input[0] == '/' && input[1] == '/';
}

std::pair<std::string, std::string> StringCalculator::splitHeaderAndNumbers(
    const std::string& input) const {
  const auto pos = input.find('\n');
  if (pos == std::string::npos) {
    // Malformed header: treat as no numbers (yields 0).
    return {input.substr(2), ""};  //  two spaces before comment is preferred
  }
  // Strip leading "//"
  return {input.substr(2, pos - 2), input.substr(pos + 1)};
}

static std::vector<std::string> extractBracketedDelims(const std::string& header) {
  std::vector<std::string> delims;
  std::regex rx(R"(\[(.*?)\])");
  for (std::sregex_iterator it(header.begin(), header.end(), rx), end; it != end; ++it) {
    const auto d = (*it).str(1);
    if (!d.empty()) delims.push_back(d);
  }
  return delims;  //  may be empty => defaults only
}

static std::vector<std::string> extractSingleCharDelim(const std::string& header) {
  if (header.size() == 1) return {header};
  return {};  //  ignore invalid unbracketed multi-char (e.g., "***")
}

std::vector<std::string> StringCalculator::extractDelimiters(
    const std::string& header) const {
  if (isBracketedHeader(header)) return extractBracketedDelims(header);
  return extractSingleCharDelim(header);
}

std::vector<std::string> StringCalculator::tokenize(
    const std::string& payload, const std::vector<std::string>& delimiters) const {
  if (payload.empty()) return {};
  std::regex splitter(joinAlternation(delimiters));
  std::sregex_token_iterator it(payload.begin(), payload.end(), splitter, -1), end;
  return {it, end};  //  empties allowed; filtered in parseNumbers
}

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

void StringCalculator::validateNoNegatives(const std::vector<int>& numbers) const {
  std::vector<int> negs;
  for (int n : numbers) {
    if (n < 0) negs.push_back(n);
  }
  if (negs.empty()) return;

  std::ostringstream oss;
  oss << "negatives not allowed: ";
  for (size_t i = 0; i < negs.size(); ++i) {
    if (i) oss << ", ";
    oss << negs[i];
  }
  throw NegativeNumberException(oss.str());
}
