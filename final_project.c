#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function Prototypes
int getValue(char roman);
int romanToDecimal(const char *roman);
void numberToWords(int num, char *result);
void processFile(const char *input, const char *output);
void displayResult(const char *operand1, int decimal1, const char *operand2, int decimal2, char operator, int result, const char *wordsResult);

// Function to get the decimal value of a Roman numeral character
int getValue(char roman) {
    switch (toupper(roman)) {
        case 'I': return 1;
        case 'V': return 5;
        case 'X': return 10;
        case 'L': return 50;
        case 'C': return 100;
        case 'D': return 500;
        case 'M': return 1000;
        default: return 0; // Invalid Roman numeral
    }
}

// Function to convert Roman numeral string to decimal
int romanToDecimal(const char *roman) {
    int total = 0, prevValue = 0;
    while (*roman) {
        int currentValue = getValue(*roman);
        if (currentValue == 0) return -1; // Invalid Roman numeral

        if (currentValue > prevValue) {
            total += currentValue - 2 * prevValue; // Subtraction case
        } else {
            total += currentValue;
        }
        prevValue = currentValue;
        roman++;
    }
    return total;
}

// Function to convert decimal number to words
void numberToWords(int num, char *result) {
    const char *ones[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
    const char *teens[] = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    const char *tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    const char *thousands[] = {"", "Thousand", "Million", "Billion"};

    if (num == 0) {
        strcpy(result, "Zero");
        return;
    }

    char temp[256] = "";
    int place = 0;
    int isNegative = 0;

    if (num < 0) {
        isNegative = 1;
        num = -num;
    }

    while (num > 0) {
        int part = num % 1000;

        if (part > 0) {
            char chunk[256] = "";

            if (part / 100 > 0) {
                strcat(chunk, ones[part / 100]);
                strcat(chunk, " Hundred");
                part %= 100;
                if (part > 0) strcat(chunk, " ");
            }

            if (part >= 10 && part < 20) {
                strcat(chunk, teens[part - 10]);
            } else {
                if (part / 10 > 0) {
                    strcat(chunk, tens[part / 10]);
                    if (part % 10 > 0) strcat(chunk, " ");
                }
                if (part % 10 > 0) {
                    strcat(chunk, ones[part % 10]);
                }
            }

            if (place > 0 && strlen(thousands[place]) > 0) {
                strcat(chunk, " ");
                strcat(chunk, thousands[place]);
            }

            if (strlen(temp) > 0) {
                strcat(chunk, " ");
            }
            strcat(chunk, temp);
            strcpy(temp, chunk);
        }

        num /= 1000;
        place++;
    }

    // Add "Negative" at the start if the number was negative
    if (isNegative) {
        char negTemp[256] = "Negative ";
        strcat(negTemp, temp);
        strcpy(temp, negTemp);
    }

    // Remove trailing spaces
    int len = strlen(temp);
    while (len > 0 && temp[len - 1] == ' ') {
        temp[len - 1] = '\0';
        len--;
    }

    strcpy(result, temp);
}

// Function to process the input and output files
void processFile(const char *input, const char *output) {
    FILE *inputFile = fopen(input, "r");
    FILE *outputFile = fopen(output, "w");

    if (!inputFile) {
        perror("Error opening input file");
        exit(1);
    }
    if (!outputFile) {
        perror("Error opening output file");
        fclose(inputFile);
        exit(1);
    }

    char line[256];
    while (fgets(line, sizeof(line), inputFile)) {
        char operand1[100], operand2[100], wordsResult[256];
        char operator;
        int decimal1, decimal2, result;

        // Parse the line
        if (sscanf(line, "%s %c %s", operand1, &operator, operand2) == 3) {
            decimal1 = romanToDecimal(operand1);
            decimal2 = romanToDecimal(operand2);

            if (decimal1 == -1 || decimal2 == -1) {
                fprintf(outputFile, "Invalid Roman numeral: %s\n", line);
                continue;
            }

            // Perform the arithmetic operation
            switch (operator) {
                case '+': result = decimal1 + decimal2; break;
                case '-': result = decimal1 - decimal2; break;
                case '*': result = decimal1 * decimal2; break;
                case '/':
                    if (decimal2 == 0) {
                        fprintf(outputFile, "Error: Division by zero: %s\n", line);
                        continue;
                    }
                    result = decimal1 / decimal2;
                    break;
                default:
                    fprintf(outputFile, "Invalid operator: %c in line: %s\n", operator, line);
                    continue;
            }

            // Convert the result to words
            numberToWords(result, wordsResult);
            fprintf(outputFile, "%s\n", wordsResult);

            // Optionally display the result on the console
            displayResult(operand1, decimal1, operand2, decimal2, operator, result, wordsResult);

        } else {
            fprintf(outputFile, "Invalid input format: %s\n", line);
        }
    }

    fclose(inputFile);
    fclose(outputFile);
    printf("Processing complete.\n");
}

// Function to display results on the console
void displayResult(const char *operand1, int decimal1, const char *operand2, int decimal2, char operator, int result, const char *wordsResult) {
    printf("%s (%d) %c %s (%d) = %d (%s)\n", operand1, decimal1, operator, operand2, decimal2, result, wordsResult);
}

// Main function
int main() {
    processFile("input.txt", "output.txt");
    return 0;
}