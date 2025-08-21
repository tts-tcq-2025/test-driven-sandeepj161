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

