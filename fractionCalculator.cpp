// File: CS112_A2_T3_S1_20230376_20230517_20230278
// Purpose: A calculator that can take whole integers of fractions and do operations on them
// Author: Mahmoud Mohamed El-Basel,
// Emails: modyelbasel50@gmail.com,
// ID1: 20230376, Did the input validation and the extraction of the numbers and the operation
// ID2: 20230517,
// ID3: 20230278,
#include <iostream>
#include <regex>
#include <iterator>
#include <cctype>

void mixedNumberOutput(int numerator, int denominator) {
    
    if (numerator > denominator) {
        int result = 0, new_num;
        new_num = numerator;

        while (new_num >= denominator) {
            new_num -= denominator;
            result += 1;
        }
        if (new_num != 0){
            std::cout << result << " " << new_num << "/" << denominator;
        }
        else{
            std::cout << result;
        }
    }
    else if (denominator == 1)
    {
        std::cout << numerator;
    }
    else{
        std::cout << numerator << "/" << denominator;
    }
}

void simplify(int numerator, int denominator) {
    for (int i = denominator; i > 0; i--) {
        if (numerator % i == 0 && denominator % i == 0) {
            numerator /= i;
            denominator /= i;
        }
    }
    mixedNumberOutput(numerator, denominator);
}

void fraction_to_double(std::string fraction,int& numerator,int& denominator)
{
    fraction += ' ';
    std::string number_string = "";
    numerator = 0, denominator = 1;
    int n = 1, number_int = 0;
    for (int i = 0; i < fraction.size(); i++)
    {
        if (fraction[i] == '/' || (n == 1 && fraction[i] == ' '))
        {
            number_int = stoi(number_string);
            numerator += number_int;
            number_string = "";
            ++n;
        }
        else if ( fraction[i] == ' ' && n == 2)
        {
            number_int = stoi(number_string);
            denominator = number_int;
        }
        else number_string += fraction[i];
    }
    if (denominator == 0) numerator = denominator = -1;
}
void operations(std::string& fraction1, std::string& fraction2, std::string& operation)
{
    int ans_numerator{ 0 }, ans_denominator{ 0 };
    int numerator1{ 0 }, denominator1{ 0 }, numerator2{ 0 }, denominator2{ 0 };
    fraction_to_double(fraction1 ,numerator1, denominator1);
    fraction_to_double(fraction2, numerator2, denominator2);
    if (denominator1 == -1 || denominator2 == -1)
    {
        std::cout << "\adenominator cant equal zero";
        return;
    }
    else if (operation == "+")
    {
        ans_numerator = denominator2 * numerator1 + denominator1 * numerator2;
        ans_denominator = denominator1 * denominator2;
    }
    else if (operation == "-")
    {
        ans_numerator = denominator2 * numerator1 - denominator1 * numerator2;
        ans_denominator = denominator1 * denominator2;
    }
    else if (operation == "*")
    {
         ans_numerator = numerator1 * numerator2;
         ans_denominator = denominator1 * denominator2;
    }
    else if (operation == "/")
    {
        ans_numerator = numerator1 * denominator2;
        ans_denominator = denominator1 * numerator2;
    }
    if (ans_denominator < 0)
    {
        ans_denominator = -ans_denominator;
        ans_numerator = -ans_numerator;
    }
    simplify(ans_numerator, ans_denominator);
}    

// here we reached that the input contains the valid digits and symbol we want and now extract them
int getNumber(std::string input, std::string& fraction1, std::string& fraction2, std::string& operation)
{
    std::smatch match{};
    input.push_back(' ');
    // as we change the input when we match a pattern to continue searching the string
    // we will need the old input also so 'originalInput' is used to save the user input
    std::string originalInput{ input };
    // fractionPattern searches for any fraction that might contain negative number
    // or search for a whole integer
    std::regex fractionPattern("([-]?[0-9]+/[-]?[0-9]+|[-]?[0-9]+)");
    // operationPattern searches for the operation that the user inputted
    std::regex operationPattern("\\s[/+*-]\\s");
    // nmrtrWithoutDeno checks if the user entered a numerator and put the fraction symbol
    // and did not provide a denominator
    std::regex nmrtrWithoutDeno{ "[-]?[0-9]+/[-]?[^-0-9]" };
    int numberOfOperands{0};
    while (std::regex_search(input, match, fractionPattern))
    {
        if (std::regex_search(input, nmrtrWithoutDeno))
        {
            std::cout << "Invalid input, numerator without denominator\n";
            return -1;
        }
        if (fraction1.length() == 0)
        {
            fraction1 = match[0];
            input = match.suffix();
            ++numberOfOperands;
            continue;
        }
        if (fraction1.length() != 0)
        {
            fraction2 = match[0];
            input = match.suffix();
            ++numberOfOperands;
            continue;
        }
    }
    // check if the user inputted more than two operands
    if(numberOfOperands > 2)
    {
        std::cout << "please enter two operands only and one operation\n";
        return -1;
    }
    match = {};
    input = originalInput;
    // check if the input contains more than one operation
    int numberOfOperations{0};
    while (std::regex_search(input, match,operationPattern))
    {
        ++numberOfOperations;
        input = match.suffix();
    }
    if(numberOfOperations > 1)
    {
        std::cout << "please enter two operands only and one operation\n";
        return -1;
    }
    // restore the old input and start to search again for the operation
    // after making sure that there is only one operation
    input = originalInput;
    if (std::regex_search(input, match, operationPattern))
    {
        operation = match[0];
    }
    else
    {
        std::cout << "Please enter a valid operation and put a space before the operation and after it\n";
        std::cout << "or you did not enter an operation\n";
        return -1;
    }
    // when we get the operation sequence it gets a space before it and a space after it
    // so select the second element which is the desired operation
    operation = operation[1];
    return 0;

}

// check if the input contain "exit" only or any bad input
int validateInput(std::string input)
{
    // search for the patter "exit" that starts in the input and make it case-insensitive
    // and doesn't contain any other words or digits
    std::regex exit{ "^exit[ ]*", std::regex::icase };
    std::smatch match{};
    if (std::regex_match(input, match, exit))
    {
        return 0;
    }
    // search for any sequence in the input that contains anything not a digit
    // or not one of the following symbols (*, +, /, -)
    std::regex reg{"[^-?0-9\\s/+*-]+" };
    if (std::regex_search(input, match, reg))
    {
        std::cout << "Please enter a valid input\n";
        std::cout << "Input should contain two rational numbers only ";
        std::cout << "and one operation of the following (+,-,/,*)\n";
        std::cout << "or enter the word \"exit\" only to exit the program\n";
        return -1;
    }
    // check that the input starts with a number and not an arbitrary symbol from the allowed
    if (!std::regex_search(input, std::regex("^([-]?[0-9]+)")))
    {
        std::cout << "Please enter a valid input\n";
        std::cout << "Input should start with a number\n";
        return -1;
    }
    // check that the input doesn't contain repeated symbols one after the other
    // as it doesn't have a meaning
    std::regex operations{ "([/+*]{2,}|[-]{2,})" };
    if (std::regex_search(input, match, operations))
    {
        std::cout << "Please enter a valid input\n";
        std::cout << match.prefix() << '[' << match.str() << ']' << match.suffix();
        std::cout << " the thing between [] is not the valid input \n";
        return -1;
    }

    return 1;
}

int main()
{
    std::string fraction1{};
    std::string fraction2{};
    std::string operation{};
    std::string input{};
    std::cout << "----------------------------------\n";
    std::cout << "| Welcome to Fraction Calculator |\n";
    std::cout << "----------------------------------";
    while (true)
    {
        std::cout << "\nPlease enter a rational number operations (or exit)\n";
        std::getline(std::cin, input);

        switch (validateInput(input)) {
            case -1:
                continue;
            case 1:
                if (getNumber(input, fraction1, fraction2, operation) == -1)
                {
                    fraction1 = fraction2 = "";
                    continue;
                }
                operations(fraction1, fraction2, operation);
                fraction2 = fraction1 = "";
                continue;
            case 0:
                std::cout << "good bye\n";
                return 0;
        }
        break;
    }
}