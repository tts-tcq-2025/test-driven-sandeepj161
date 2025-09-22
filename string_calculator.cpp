#include "./string_calculator.h"

#include <algorithm>
#include <numeric>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

// Public API
int StringCalculator::add(const std::string& input) {
  if (input.empty()) return 0;

  auto tokens = tokenize(input);
  auto numbers = convertToNumbers(tokens);
  validateNumbers(numbers);
  return sumNumbers(numbers);
}

std::vector<std::string> StringCalculator::tokenize(const std::string& input) {
  std::string delimiters = ",\n";
  std::string numbersPart = input;

  if (input.rfind("//", 0) == 0) {
    auto newlinePos = input.find('\n');
    if (newlinePos != std::string::npos) {
      delimiters = input.substr(2, newlinePos - 2);
      numbersPart = input.substr(newlinePos + 1);
    }
  }

  std::vector<std::string> tokens;
  std::string token;
  for (char c : numbersPart) {
    if (delimiters.find(c) != std::string::npos) {
      if (!token.empty()) {
        tokens.push_back(token);
        token.clear();
      }
    } else {
      token.push_back(c);
    }
  }
  if (!token.empty()) tokens.push_back(token);

  return tokens;
}

std::vector<int> StringCalculator::convertToNumbers(
    const std::vector<std::string>& tokens) {
  std::vector<int> numbers;
  numbers.reserve(tokens.size());

  for (const auto& token : tokens) {
    if (!token.empty()) {
      numbers.push_back(std::stoi(token));
    }
  }
  return numbers;
}

void StringCalculator::validateNumbers(const std::vector<int>& numbers) {
  std::vector<int> negatives;
  std::copy_if(numbers.begin(), numbers.end(), std::back_inserter(negatives),
               [](int n) { return n < 0; });

  if (!negatives.empty()) {
    std::ostringstream oss;
    oss << "Negatives not allowed: ";
    for (size_t i = 0; i < negatives.size(); ++i) {
      if (i > 0) oss << ", ";
      oss << negatives[i];
    }
    throw NegativeNumberException(oss.str());
  }
}

int StringCalculator::sumNumbers(const std::vector<int>& numbers) {
  return std::accumulate(numbers.begin(), numbers.end(), 0,
                         [](int acc, int n) {
                           return acc + ((n <= kMaxNumber) ? n : 0);
                         });
}
