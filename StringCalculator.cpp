#include "StringCalculator.h"
#include <algorithm>
#include <sstream>

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
            // Extract delimiter, e.g., ";" or "***"
            std::string delim = input.substr(2, delimiterEnd - 2);
            // Remove brackets if present
            delim.erase(std::remove(delim.begin(), delim.end(), '['), delim.end());
            delim.erase(std::remove(delim.begin(), delim.end(), ']'), delim.end());
            return delim;
        }
    }
    return ",";  // Default
}

std::string StringCalculator::getNumbersPart(const std::string& input, const std::string& delimiter) {
    if (input.rfind("//", 0) == 0) {
        size_t newlinePos = input.find("\n");
        if (newlinePos != std::string::npos) {
            return input.substr(newlinePos + 1);
        }
    }
    // Handle newlines by replacing them with the delimiter
    std::string result = input;
    size_t pos = 0;
    while ((pos = result.find("\n", pos)) != std::string::npos) {
        result.replace(pos, 1, delimiter);
        pos += delimiter.length();
    }
    return result;
}

int StringCalculator::sumNumbers(const std::string& numbers, const std::string& delimiter) {
    std::vector<std::string> parts = split(numbers, delimiter);
    std::vector<int> negatives;
    int sum = 0;

    for (const auto& part : parts) {
        if (part.empty()) continue;
        std::istringstream iss(part);
        int num;
        iss >> num;  // Simple parse (assumes valid input as per kata)
        if (num < 0) {
            negatives.push_back(num);
        } else if (num <= 1000) {
            sum += num;
        }
    }

    if (!negatives.empty()) {
        std::stringstream msg;
        msg << "negatives not allowed:";
        for (size_t i = 0; i < negatives.size(); ++i) {
            msg << " " << negatives[i];
            if (i < negatives.size() - 1) msg << ",";
        }
        throw std::invalid_argument(msg.str());
    }

    return sum;
}

std::vector<std::string> StringCalculator::split(const std::string& str, const std::string& delimiter) {
    std::vector<std::string> tokens;
    std::string s = str;
    size_t pos = 0;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        tokens.push_back(s.substr(0, pos));
        s.erase(0, pos + delimiter.length());
    }
    tokens.push_back(s);  // Last token
    return tokens;
}
