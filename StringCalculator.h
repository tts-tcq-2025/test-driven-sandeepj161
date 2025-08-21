#ifndef STRINGCALCULATOR_H_
#define STRINGCALCULATOR_H_

#include <string>
#include <vector>
#include <stdexcept>

class StringCalculator {
 public:
  int add(const std::string& input);

 private:
  std::string getDelimiter(const std::string& input);
  std::string getNumbersPart(const std::string& input,
                             const std::string& delimiter);
  std::string extractNumbersAfterPrefix(const std::string& input);
  std::string replaceNewlinesWithDelimiter(const std::string& input,
                                           const std::string& delimiter);
  int sumNumbers(const std::string& numbers, const std::string& delimiter);
  int parseNumber(const std::string& part);
  int collectNegativesAndSum(const std::vector<std::string>& parts,
                             std::vector<int>& negatives);
  std::string buildNegativesErrorMessage(const std::vector<int>& negatives);
  std::vector<std::string> split(const std::string& str,
                                 const std::string& delimiter);
};

#endif
