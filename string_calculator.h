#ifndef STRING_CALCULATOR_H
#define STRING_CALCULATOR_H

#include <string>
#include <vector>
#include <stdexcept>

class NegativeNumberException : public std::runtime_error {
public:
    explicit NegativeNumberException(const std::string& message)
        : std::runtime_error(message) {}
};

class StringCalculator {
public:
    // Core public API: pure w.r.t. input string
    int add(const std::string& input);

private:
    // Header parsing
    bool hasCustomDelimiter(const std::string& input) const;
    std::pair<std::string, std::string> splitHeaderAndNumbers(const std::string& input) const;

    // Delimiter extraction (extensible seam)
    std::vector<std::string> extractDelimiters(const std::string& header) const;

    // Tokenization and parsing
    std::vector<std::string> tokenize(const std::string& payload,
                                      const std::vector<std::string>& delimiters) const;
    std::vector<int> parseNumbers(const std::vector<std::string>& tokens) const;

    // Rules (extensible seam)
    void validateNoNegatives(const std::vector<int>& numbers) const;
};

#endif
