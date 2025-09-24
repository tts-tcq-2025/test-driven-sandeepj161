#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "StringCalculator.h"

// Spec scenarios (from README)
TEST_CASE("Empty input returns 0") {
    StringCalculator calc;
    REQUIRE(calc.add("") == 0);
}

TEST_CASE("Single number returns its value") {
    StringCalculator calc;
    REQUIRE(calc.add("1") == 1);
}

TEST_CASE("Two numbers (comma-separated)") {
    StringCalculator calc;
    REQUIRE(calc.add("1,2") == 3);
}

TEST_CASE("Arbitrary count of numbers") {
    StringCalculator calc;
    REQUIRE(calc.add("1,2,3,4") == 10);
}

TEST_CASE("Newlines as delimiters (with commas)") {
    StringCalculator calc;
    REQUIRE(calc.add("1\n2,3") == 6);
}

TEST_CASE("Custom single-character delimiter") {
    StringCalculator calc;
    REQUIRE(calc.add("//;\n1;2") == 3);
}

TEST_CASE("Custom multi-character delimiter") {
    StringCalculator calc;
    REQUIRE(calc.add("//[***]\n1***2***3") == 6);
}

TEST_CASE("Negative number throws with message (single)") {
    StringCalculator calc;
    REQUIRE_THROWS_WITH(calc.add("1,-2,3"), "negatives not allowed: -2");
}

TEST_CASE("Multiple negatives throw with full list") {
    StringCalculator calc;
    REQUIRE_THROWS_WITH(calc.add("1,-2,-3"), "negatives not allowed: -2, -3");
}

TEST_CASE("Numbers greater than 1000 are ignored") {
    StringCalculator calc;
    REQUIRE(calc.add("2,1001") == 2);
}

TEST_CASE("Boundary at 1000 is included") {
    StringCalculator calc;
    REQUIRE(calc.add("2,1000") == 1002);
}

TEST_CASE("Header is optional (defaults apply without it)") {
    StringCalculator calc;
    REQUIRE(calc.add("1,2,3") == 6);
}

TEST_CASE("Custom header at start defines delimiter") {
    StringCalculator calc;
    REQUIRE(calc.add("//;\n1;2,3") == 6); // header sets ';' but commas still supported
}

TEST_CASE("Multi-character delimiter with spaces") {
    StringCalculator calc;
    REQUIRE(calc.add("//[ab cd]\n1ab cd2ab cd3") == 6);
}

TEST_CASE("Mixed default delimiters without header") {
    StringCalculator calc;
    REQUIRE(calc.add("4\n5,6") == 15);
}

TEST_CASE("Custom delimiter with multiple values") {
    StringCalculator calc;
    REQUIRE(calc.add("//|\n1|2|3") == 6);
}

TEST_CASE("Single value with custom delimiter") {
    StringCalculator calc;
    REQUIRE(calc.add("//;\n7") == 7);
}

TEST_CASE("All values greater than 1000 yield zero") {
    StringCalculator calc;
    REQUIRE(calc.add("1001,2000") == 0);
}

TEST_CASE("Negative at start throws with message") {
    StringCalculator calc;
    REQUIRE_THROWS_WITH(calc.add("-1,2,3"), "negatives not allowed: -1");
}

// ---------------- DFV (Delimiter Format Validation) test cases ----------------

// DFV01 Header not at start is ignored
// Input: "1,2\n//;\n3;4"
// The README expects header must be at very beginning to take effect.
// In this implementation, header in payload will be treated as plain text,
// and the stray ';' will remain in the token "3;4" causing stoi to throw.
// We assert the implementation throws (robustness: malformed payload).
TEST_CASE("DFV01 Header not at start is ignored => payload malformed leads to exception") {
    StringCalculator calc;
    REQUIRE_THROWS(calc.add("1,2\n//;\n3;4"));
}

// DFV02 Missing newline after header
// Input: "//;1;2" — spec says undefined; we fall back to returning 0 (no numbers parsed).
TEST_CASE("DFV02 Missing newline after header leads to 0 (fallback)") {
    StringCalculator calc;
    REQUIRE(calc.add("//;1;2") == 0);
}

// DFV03 Empty delimiter brackets
// Input: "//[]\n1,2" — treat as default delimiters.
TEST_CASE("DFV03 Empty delimiter brackets treated as default delimiters") {
    StringCalculator calc;
    REQUIRE(calc.add("//[]\n1,2") == 3);
}

// DFV04 Multi-character delimiter bracketed
TEST_CASE("DFV04 Multi-character delimiter bracketed") {
    StringCalculator calc;
    REQUIRE(calc.add("//[***]\n1***2***3") == 6);
}

// DFV05 Multi-character delimiter without []
// Input: "//***\n1***2***3" — our implementation treats header as single-char '*' and splits by '*'.
// The result will still parse numbers (consecutive '*' produce empty tokens which are skipped).
// Expect 6 (this behavior is implementation-defined).
TEST_CASE("DFV05 Multi-character delimiter without brackets -> implementation-defined (we get 6)") {
    StringCalculator calc;
    REQUIRE(calc.add("//***\n1***2***3") == 6);
}

// DFV06 Single-character custom delimiter
TEST_CASE("DFV06 Single-character custom delimiter") {
    StringCalculator calc;
    REQUIRE(calc.add("//;\n1;2;3") == 6);
}

// DFV07 Header with spaces in delimiter
TEST_CASE("DFV07 Header with spaces in delimiter") {
    StringCalculator calc;
    REQUIRE(calc.add("//[  **  ]\n1  **  2") == 3);
}

// DFV08 Mixed default delimiters without header
TEST_CASE("DFV08 Mixed default delimiters without header") {
    StringCalculator calc;
    REQUIRE(calc.add("1\n2,3") == 6);
}

// DFV09 Mixed with custom header in payload
TEST_CASE("DFV09 Mixed with custom header in payload (header at start applies)") {
    StringCalculator calc;
    REQUIRE(calc.add("//;\n1;2,3") == 6);
}

// DFV10 Trailing delimiter (clarification only)
// Input: "1,\n" — spec says not required. Our implementation treats empty tokens as skipped and returns 1.
TEST_CASE("DFV10 Trailing delimiter => treat empty tokens as skipped (result 1)") {
    StringCalculator calc;
    REQUIRE(calc.add("1,\n") == 1);
}

// DFV11 Multiple headers not supported
// We assert that header must be at start; additional occurrences in payload are treated as text.
// This typically leads to malformed payload parsing and an exception in our implementation.
TEST_CASE("DFV11 Multiple headers not supported -> payload header ignored and may throw") {
    StringCalculator calc;
    REQUIRE_THROWS(calc.add("//;\n//\n1\t2"));
}

// DFV12 Header with missing closing bracket
// Input: "//[**\n1**2**3" -> malformed bracket header; choose to treat as single-char delimiter,
// which will not split correctly; result is implementation-defined; expect it to throw.
TEST_CASE("DFV12 Header with missing closing bracket -> malformed, expect exception") {
    StringCalculator calc;
    REQUIRE_THROWS(calc.add("//[**\n1**2**3"));
}

// DFV13 Empty numbers line after header
TEST_CASE("DFV13 Empty numbers line after header yields 0") {
    StringCalculator calc;
    REQUIRE(calc.add("//;\n") == 0);
}

// DFV14 Whitespace around header delimiter (spaces inside brackets are part of delimiter)
TEST_CASE("DFV14 Whitespace around header delimiter is part of delimiter") {
    StringCalculator calc;
    REQUIRE(calc.add("//[  **  ]\n1  **  2") == 3);
}

// DFV15 Default delimiters with custom present
TEST_CASE("DFV15 Default delimiters still work when custom delimiter present") {
    StringCalculator calc;
    REQUIRE(calc.add("//;\n1;2,3") == 6);
}
