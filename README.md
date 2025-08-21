# TDD Driven StringCalculator

Build a StringCalculator functionality that can take up to two numbers, separated by commas, and will return their sum. 
for example “” or “1” or “1,2” as inputs.

> DO NOT jump into implementation! Read the example and the starting task below.

- For an empty string it will return 0
- Allow the Add method to handle an unknown amount of numbers
- Allow the Add method to handle new lines between numbers (instead of commas).
  - the following input is ok: “1\n2,3” (will equal 6)
  - the following input is NOT ok: “1,\n” (not need to prove it - just clarifying)
- Support different delimiters : to change a delimiter, the beginning of the string will contain a separate line that looks like this: “//[delimiter]\n[numbers…]” for example “//;\n1;2” should return three where the default delimiter is ‘;’ .
the first line is optional. all existing scenarios should still be supported
- Calling Method with a negative number will throw an exception “negatives not allowed” - and the negative that was passed. if there are multiple negatives, show all of them in the exception message.
- Numbers bigger than 1000 should be ignored, so adding 2 + 1001 = 2
- Delimiters can be of any length with the following format: “//[delimiter]\n” for example: “//[***]\n1***2***3” should return 6

## Tasks



Establish quality parameters:

- Ensure  maximum complexity (CCN) per function == 3

- Ensure 100% line and branch coverage at every step

  

Start Test-driven approach

1. Write the smallest possible failing test: give input `"" assert output to be 0 ` .
2. Write the minimum amount of code that'll make it pass.
3. Refactor any assumptions, continue to pass this test. Do not add any code without a corresponding test.

## Test Specifications

1) Empty input returns 0  
- Input: `""`  
- Action: `add`  
- Expected: `0`

2) Single number returns its value  
- Input: `"1"`  
- Action: `add`  
- Expected: `1`

3) Two numbers (comma-separated)  
- Input: `"1,2"`  
- Action: `add`  
- Expected: `3`

4) Arbitrary count of numbers  
- Input: `"1,2,3,4"`  
- Action: `add`  
- Expected: `10`

5) Newlines as delimiters (with commas)  
- Input: `"1\n2,3"`  
- Action: `add`  
- Expected: `6`

6) Custom single-character delimiter  
- Input: `"//;\n1;2"`  
- Action: `add`  
- Expected: `3`

7) Custom multi-character delimiter  
- Input: `"//[***]\n1***2***3"`  
- Action: `add`  
- Expected: `6`

8) Negative number throws with message (single)  
- Input: `"1,-2,3"`  
- Action: `add`  
- Expected: Exception with message: `negatives not allowed: -2`

9) Multiple negatives throw with full list  
- Input: `"1,-2,-3"`  
- Action: `add`  
- Expected: Exception with message: `negatives not allowed: -2, -3`

10) Numbers >1000 are ignored  
- Input: `"2,1001"`  
- Action: `add`  
- Expected: `2`

11) Boundary at 1000 is included  
- Input: `"2,1000"`  
- Action: `add`  
- Expected: `1002`

12) Header is optional (defaults apply without it)  
- Input: `"1,2,3"`  
- Action: `add`  
- Expected: `6`

13) Custom header at start defines delimiter  
- Input: `"//;\n1;2,3"`  
- Action: `add`  
- Expected: `6`

14) Multi-character delimiter with spaces  
- Input: `"//[ab cd]\n1ab cd2ab cd3"`  
- Action: `add`  
- Expected: `6`

15) Clarification-only (invalid tokenization not required)  
- Input: `"1,\n"`  
- Action: `add`  
- Expected: Not required by spec (no test)

16) Mixed default delimiters without header  
- Input: `"4\n5,6"`  
- Action: `add`  
- Expected: `15`

17) Custom delimiter with multiple values  
- Input: `"//|\n1|2|3"`  
- Action: `add`  
- Expected: `6`

18) Single value with custom delimiter  
- Input: `"//;\n7"`  
- Action: `add`  
- Expected: `7`

19) All values >1000 yield zero  
- Input: `"1001,2000"`  
- Action: `add`  
- Expected: `0`

20) Negative at start throws with message  
- Input: `"-1,2,3"`  
- Action: `add`  
- Expected: Exception with message: `negatives not allowed: -1`

## Gherkin

Feature: StringCalculator - Test Specifications

  Background:
    Given a fresh StringCalculator instance

  Scenario: Empty input returns 0
    Given the input ""
    When I call add
    Then the result should be 0

  Scenario: Single number returns its value
    Given the input "1"
    When I call add
    Then the result should be 1

  Scenario: Two numbers (comma-separated)
    Given the input "1,2"
    When I call add
    Then the result should be 3

  Scenario: Arbitrary count of numbers
    Given the input "1,2,3,4"
    When I call add
    Then the result should be 10

  Scenario: Newlines as delimiters (with commas)
    Given the input "1\n2,3"
    When I call add
    Then the result should be 6

  Scenario: Custom single-character delimiter
    Given the input "//;\n1;2"
    When I call add
    Then the result should be 3

  Scenario: Custom multi-character delimiter
    Given the input "//[***]\n1***2***3"
    When I call add
    Then the result should be 6

  Scenario: Negative number throws with message (single)
    Given the input "1,-2,3"
    When I call add
    Then an exception should be thrown with message "negatives not allowed: -2"

  Scenario: Multiple negatives throw with full list
    Given the input "1,-2,-3"
    When I call add
    Then an exception should be thrown with message "negatives not allowed: -2, -3"

  Scenario: Numbers greater than 1000 are ignored
    Given the input "2,1001"
    When I call add
    Then the result should be 2

  Scenario: Boundary at 1000 is included
    Given the input "2,1000"
    When I call add
    Then the result should be 1002

  Scenario: Header is optional (defaults apply without it)
    Given the input "1,2,3"
    When I call add
    Then the result should be 6

  Scenario: Custom header at start defines delimiter
    Given the input "//;\n1;2,3"
    When I call add
    Then the result should be 6

  Scenario: Multi-character delimiter with spaces
    Given the input "//[ab cd]\n1ab cd2ab cd3"
    When I call add
    Then the result should be 6

  # Clarification-only; not required by the spec to validate
  Scenario: Clarification-only invalid tokenization not required
    Given the input "1,\n"
    When I call add
    Then no validation is required by the specification

  Scenario: Mixed default delimiters without header
    Given the input "4\n5,6"
    When I call add
    Then the result should be 15

  Scenario: Custom delimiter with multiple values
    Given the input "//|\n1|2|3"
    When I call add
    Then the result should be 6

  Scenario: Single value with custom delimiter
    Given the input "//;\n7"
    When I call add
    Then the result should be 7

  Scenario: All values greater than 1000 yield zero
    Given the input "1001,2000"
    When I call add
    Then the result should be 0

  Scenario: Negative at start throws with message
    Given the input "-1,2,3"
    When I call add
    Then an exception should be thrown with message "negatives not allowed: -1"

## Delimiter Format Validation – Test Scenarios

| Test Case ID | Name                                   | Precondition              | Input                      | Action | Expected Output / Exception                          | Comment                                                |
|--------------|----------------------------------------|---------------------------|----------------------------|--------|------------------------------------------------------|--------------------------------------------------------|
| DFV01        | Header not at start is ignored         | Calculator instance ready | `"1,2\n//;\n3;4"`          | add    | `10`                                                 | Header must be at the very beginning to take effect.  |
| DFV02        | Missing newline after header           | Calculator instance ready | `"//;1;2"`                 | add    | `0` or undefined by spec (not required to validate)  | Header format requires `\n` after delimiter.           |
| DFV03        | Empty delimiter brackets               | Calculator instance ready | `"//[]\n1,2"`              | add    | `3`                                                  | Treats as default delimiters; empty custom ignored.    |
| DFV04        | Multi-character delimiter bracketed    | Calculator instance ready | `"//[***]\n1***2***3"`     | add    | `6`                                                  | Valid multi-char delimiter only when bracketed.        |
| DFV05        | Multi-character delimiter without []   | Calculator instance ready | `"//***\n1***2***3"`       | add    | `0` or undefined by spec (not required to validate)  | Without brackets, only single-char custom is valid.    |
| DFV06        | Single-character custom delimiter      | Calculator instance ready | `"//;\n1;2;3"`             | add    | `6`                                                  | Valid single-char custom delimiter.                    |
| DFV07        | Header with spaces in delimiter        | Calculator instance ready | `"//[ab cd]\n1ab cd2ab cd3"` | add  | `6`                                                  | Spaces allowed in bracketed multi-char delimiter.      |
| DFV08        | Mixed default delimiters without header| Calculator instance ready | `"1\n2,3"`                 | add    | `6`                                                  | Default comma and newline both valid without header.   |
| DFV09        | Mixed with custom header in payload    | Calculator instance ready | `"//;\n1;2,3"`             | add    | `6`                                                  | After header, payload can still include numbers; sum ok.|
| DFV10        | Trailing delimiter (clarification only)| Calculator instance ready | `"1,\n"`                   | add    | Not required by spec (no test)                       | Clarification case; validator not required.            |
| DFV11        | Multiple headers not supported         | Calculator instance ready | `"//;\n//|\n1|2"`          | add    | `3`                                                  | Only the first header at start is considered.          |
| DFV12        | Header with missing closing bracket    | Calculator instance ready | `"//[**\n1**2**3"`         | add    | `0` or undefined by spec (not required to validate)  | Malformed bracketed delimiter; validation not required.|
| DFV13        | Empty numbers line after header        | Calculator instance ready | `"//;\n"`                  | add    | `0`                                                  | No numbers yields 0.                                   |
| DFV14        | Whitespace around header delimiter     | Calculator instance ready | `"//[  **  ]\n1  **  2"`   | add    | `3`                                                  | Whitespace inside brackets is part of delimiter.       |
| DFV15        | Default delimiters with custom present | Calculator instance ready | `"//;\n1;2,3"`             | add    | `6`                                                  | Custom delimiter applies; extra comma still splits.    |

