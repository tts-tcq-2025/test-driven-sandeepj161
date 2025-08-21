#include "StringCalculator.h"

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

int StringCalculator::add(const std::string& input) {
  if (input.empty()) {
    return 0;
  }

  std::string delimiter = getDelimiter(input);
  std::string numbersPart = getNumbersPart(input, delimiter);

  return sumNumbers(numbersPart, delimiter);
}

std::string StringCalculator::getDelimiter(const std::string& input) {
  if (input.rfind("//", 0) == 0) {  // Starts with "//"
    size_t delimiterEnd = input.find("\n");
    if (delimiterEnd != std::string::npos) {
      std::string delim = input.substr(2, delimiterEnd - 2);
      delim.erase(std::remove(delim.begin(), delim.end(), '['), delim.end());
      delim.erase(std::remove(delim.begin(), delim.end(), ']'), delim.end());
      return delim;
    }
  }
  return ",";  // Default
}

std::string StringCalculator::getNumbersPart(const std::string& input,
                                             const std::string& delimiter) {
  if (input.rfind("//", 0) == 0) {
    return extractNumbersAfterPrefix(input);
  }
  return replaceNewlinesWithDelimiter(input, delimiter);
}

std::string StringCalculator::extractNumbersAfterPrefix(
    const std::string& input) {
  size_t newlinePos = input.find("\n");
  if (newlinePos != std::string::npos) {
    return input.substr(newlinePos + 1);
  }
  return input;
}

std::string StringCalculator::replaceNewlinesWithDelimiter(
    const std::string& input, const std::string& delimiter) {
  std::string result = input;
  size_t pos = 0;
  while ((pos = result.find("\n", pos)) != std::string::npos) {
    result.replace(pos, 1, delimiter);
    pos += delimiter.length();
  }
  return result;
}

int StringCalculator::sumNumbers(const std::string& numbers,
                                 const std::string& delimiter) {
  std::vector<std::string> parts = split(numbers, delimiter);
  std::vector<int> negatives;
  int sum = collectNegativesAndSum(parts, negatives);

  if (!negatives.empty()) {
    std::string msg = buildNegativesErrorMessage(negatives);
    throw std::invalid_argument(msg);
  }

  return sum;
}

int StringCalculator::parseNumber(const std::string& part) {
  std::istringstream iss(part);
  int num;
  iss >> num;
  return num;
}

int StringCalculator::collectNegativesAndSum(
    const std::vector<std::string>& parts, std::vector<int>& negatives) {
  int sum = 0;
  for (const auto& part : parts) {
    processPart(part, sum, negatives);
  }
  return sum;
}

void StringCalculator::processPart(const std::string& part, int& sum,
                                   std::vector<int>& negatives) {
  if (part.empty()) return;
  int num = parseNumber(part);

  if (num < 0) {
    handleNegative(num, negatives);
    return;
  }
  handleNonNegative(num, sum);
}

void StringCalculator::handleNegative(int num, std::vector<int>& negatives) {
  negatives.push_back(num);
}

void StringCalculator::handleNonNegative(int num, int& sum) {
  if (num <= 1000) {
    sum += num;
  }
}

std::string StringCalculator::buildNegativesErrorMessage(
    const std::vector<int>& negatives) {
  std::stringstream msg;
  msg << "negatives not allowed:";
  for (size_t i = 0; i < negatives.size(); ++i) {
    msg << " " << negatives[i];
    if (i < negatives.size() - 1) msg << ",";
  }
  return msg.str();
}

std::vector<std::string> StringCalculator::split(const std::string& str,
                                                 const std::string& delimiter) {
  std::vector<std::string> tokens;
  std::string s = str;
  size_t pos = 0;
  while ((pos = s.find(delimiter)) != std::string::npos) {
    tokens.push_back(s.substr(0, pos));
    s.erase(0, pos + delimiter.length());
  }
  tokens.push_back(s);
  return tokens;
}
