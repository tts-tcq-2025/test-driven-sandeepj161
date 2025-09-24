#include "StringCalculator.h"
#include <algorithm>
#include <cctype>
#include <sstream>
#include <stdexcept>

// ------------------ NegativeNumberException ------------------

NegativeNumberException::NegativeNumberException(const std::vector<int>& negatives)
    : message_("negatives not allowed: ") {  // initialization list used
    for (size_t i = 0; i < negatives.size(); ++i) {
        message_ += std::to_string(negatives[i]);
        if (i + 1 < negatives.size()) {
            message_ += ",";
        }
    }
}

const char* NegativeNumberException::what() const noexcept {
    return message_.c_str();
}

// ------------------ StringCalculator ------------------

int StringCalculator::add(const std::string& numbers) {
    if (numbers.empty()) {
        return 0;
    }

    std::string numbersPart = numbers;
    std::vector<std::string> delimiters = {",", "\n"};

    if (numbers.rfind("//", 0) == 0) {
        delimiters = extractDelimiters(numbers, numbersPart);
    }

    auto tokens = split(numbersPart, delimiters);
    auto values = toNumbers(tokens);

    int sum = 0;
    std::vector<int> negatives;

    for (int n : values) {
        if (n < 0) {
            negatives.push_back(n);
        } else if (n <= 1000) {
            sum += n;
        }
    }

    if (!negatives.empty()) {
        throw NegativeNumberException(negatives);
    }

    return sum;
}

std::vector<std::string> StringCalculator::split(
    const std::string& input, const std::vector<std::string>& delimiters) {

    std::vector<std::string> tokens;
    size_t pos = 0, lastPos = 0;

    while (pos < input.size()) {
        auto match = std::find_if(delimiters.begin(), delimiters.end(),
                                  [&](const std::string& delim) {
                                      return input.compare(pos, delim.size(), delim) == 0;
                                  });

        if (match != delimiters.end()) {
            tokens.push_back(input.substr(lastPos, pos - lastPos));
            pos += match->size();
            lastPos = pos;
        } else {
            ++pos;
        }
    }

    tokens.push_back(input.substr(lastPos));
    return tokens;
}

std::vector<std::string> StringCalculator::extractDelimiters(
    const std::string& input, std::string& numbersPart) {

    std::vector<std::string> delimiters;
    size_t newlinePos = input.find("\n");
    std::string delimPart = input.substr(2, newlinePos - 2);
    numbersPart = input.substr(newlinePos + 1);

    if (delimPart.size() > 1 && delimPart[0] == '[') {
        size_t pos = 0;
        while (pos < delimPart.size()) {
            size_t start = delimPart.find('[', pos);
            size_t end = delimPart.find(']', start);
            if (start == std::string::npos || end == std::string::npos) break;
            delimiters.push_back(delimPart.substr(start + 1, end - start - 1));
            pos = end + 1;
        }
    } else {
        delimiters.push_back(delimPart);
    }

    return delimiters;
}

std::vector<int> StringCalculator::toNumbers(
    const std::vector<std::string>& tokens) {

    std::vector<int> result;
    for (const auto& token : tokens) {
        if (!token.empty()) {
            result.push_back(std::stoi(token));
        }
    }
    return result;
}
