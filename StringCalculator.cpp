#include "StringCalculator.h"

#include <algorithm>
#include <cctype>
#include <sstream>
#include <stdexcept>
#include <string>   // explicit include
#include <vector>   // explicit include

NegativeNumberException::NegativeNumberException(
    const std::vector<int>& negatives) {
  message_ = "negatives not allowed: ";
  for (size_t i = 0; i < negatives.size(); ++i) {
    message_ += std::to_string(negatives[i]);
    if (i != negatives.size() - 1) {
      message_ += ", ";
    }
  }
}

const char* NegativeNumberException::what() const noexcept {
  return message_.c_str();
}

int StringCalculator::add(const std::string& input) {
  if (input.empty()) {
    return 0;
  }

  std::string numbersSection = input;
  std::vector<std::string> delimiters = {",", "\n"};

  // Handle custom delimiters
  if (input.rfind("//", 0) == 0) {
    delimiters = extractDelimiters(input, numbersSection);
  }

  std::vector<std::string> tokens = split(numbersSection, delimiters);
  std::vector<int> numbers = convertToNumbers(tokens);

  std::vector<int> negatives;
  int sum = 0;
  for (int num : numbers) {
    if (num < 0) {
      negatives.push_back(num);
    } else if (num <= 1000) {
      sum += num;
    }
  }

  if (!negatives.empty()) {
    throw NegativeNumberException(negatives);
  }

  return sum;
}

std::vector<std::string> StringCalculator::extractDelimiters(
    const std::string& input, std::string& numbersSection) {
  size_t newlinePos = input.find('\n');
  std::string header = input.substr(2, newlinePos - 2);
  numbersSection = input.substr(newlinePos + 1);

  std::vector<std::string> delimiters;
  if (header.size() >= 2 && header.front() == '[' && header.back() == ']') {
    size_t pos = 0;
    while (pos < header.size()) {
      size_t start = header.find('[', pos);
      size_t end = header.find(']', start);
      if (start == std::string::npos || end == std::string::npos) break;
      delimiters.push_back(header.substr(start + 1, end - start - 1));
      pos = end + 1;
    }
  } else {
    delimiters.push_back(header);
  }

  return delimiters;
}

std::vector<std::string> StringCalculator::split(
    const std::string& input, const std::vector<std::string>& delimiters) {
  std::vector<std::string> tokens;
  size_t pos = 0;
  size_t start = 0;

  while (pos < input.size()) {
    size_t nextDelimPos = std::string::npos;
    size_t delimLen = 0;

    for (const auto& delim : delimiters) {
      if (input.compare(pos, delim.size(), delim) == 0) {
        nextDelimPos = pos;
        delimLen = delim.size();
        break;
      }
    }

    if (nextDelimPos != std::string::npos) {
      tokens.push_back(input.substr(start, pos - start));
      pos += delimLen;
      start = pos;
    } else {
      ++pos;
    }
  }

  if (start < input.size()) {
    tokens.push_back(input.substr(start));
  }

  return tokens;
}

std::vector<int> StringCalculator::convertToNumbers(
    const std::vector<std::string>& tokens) {
  std::vector<int> numbers;
  for (const auto& token : tokens) {
    if (!token.empty()) {
      numbers.push_back(std::stoi(token));
    }
  }
  return numbers;
}
