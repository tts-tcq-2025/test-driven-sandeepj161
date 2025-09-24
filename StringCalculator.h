#ifndef STRING_CALCULATOR_H
#define STRING_CALCULATOR_H

#include <string>
#include <vector>
#include <stdexcept>
#include <utility>

class NegativeNumberException : public std::runtime_error {
public:
    explicit NegativeNumberException(const std::vector<int>& negatives);

private:
    static std::string buildMessage(const std::vector<int>& negatives);
};

class StringCalculator {
public:
    // Adds numbers from the input string according to the spec.
    // Throws NegativeNumberException if negative numbers are found.
    int add(const std::string& input);

private:
    static constexpr int MAX_NUMBER = 1000;

    using DelimiterResult = std::pair<std::vector<std::string>, std::string>;

    // If header present (starting with "//"), returns (delimiters, numbersPart).
    // Otherwise returns (defaultDelimiters, whole input).
    DelimiterResult extractDelimiter(const std::string& input);

    // Parse header body into custom delimiters.
    // Accepts bracketed multi-length delimiters: e.g. [***] or [ab cd].
    // Also supports single-char delimiter (when header is single char w/o brackets).
    std::vector<std::string> parseCustomDelimiters(const std::string& header);

    // Split the numbersPart by provided delimiters (regex-based split).
    std::vector<std::string> splitNumbers(const std::string& input,
                                          const std::vector<std::string>& delimiters);

    // Convert tokens to ints, gather negatives, ignore > MAX_NUMBER.
    // Throws NegativeNumberException if negatives exist.
    std::vector<int> validateNumbers(const std::vector<std::string>& tokens);

    // Sum numbers vector.
    int sumNumbers(const std::vector<int>& numbers);

    // default delimiters are comma and newline.
    std::vector<std::string> defaultDelimiters();
};

#endif // STRING_CALCULATOR_H
