#include <iostream>
#include <regex>
#include <iterator>
#include <cctype>

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
}    

int getNumber(std::string input, std::string& fraction1, std::string& fraction2, std::string& operation)
{
    std::smatch match{};
    input.push_back(' ');
    std::string originalInput{ input };
    std::regex fractionPattern("([-]?[0-9]+/[-]?[0-9]+|[-]?[0-9]+)");
    std::regex operationPatter("\\s[/+*-]\\s");
    std::regex nmrtrWithoutDeno{ "[-]?[0-9]+/[-]?[^-0-9]" };

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
            continue;
        }
        if (fraction1.length() != 0)
        {
            fraction2 = match[0];
            input = match.suffix();
            continue;
        }
    }
    match = {};
    input = originalInput;
    if (std::regex_search(input, match, operationPatter))
    {
        operation = match[0];
    }
    else
    {
        std::cout << "Please enter a valid operation and put a space before the operation and after it\n";
        std::cout << "or you did not enter an operation\n";
        return -1;
    }
    operation = operation[1];
    return 0;

}

//std::string removeSpaces(std::string input)
//{
//    std::regex removeSpace{"\\s"};
//    std::string result{};
//    std::regex_replace(std::back_inserter(result),input.begin(),input.end(),removeSpace,"");
//    std::cout << result << '\n';
//    return result;
//}

int validateInput(std::string input)
{
    std::regex exit{ "^exit[ ]*", std::regex::icase };
    std::smatch match{};
    if (std::regex_match(input, match, exit))
    {
        return 0;
    }
    std::regex reg{"[^-?0-9\\s/+*-]+" };
    if (std::regex_search(input, match, reg))
    {
        std::cout << "Please enter a valid input\n";
        std::cout << "Input should contain two rational numbers only ";
        std::cout << "and one operation of the following (+,-,/,*)\n";
        std::cout << "or enter the word \"exit\" only to exit the program\n";
        return -1;
    }
    if (!std::regex_search(input, std::regex("^([-]?[0-9]+)")))
    {
        std::cout << "Please enter a valid input\n";
        std::cout << "Input should start with a number\n";
        return -1;
    }
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
    while (true)
    {
        std::cout << "Please enter a rational number operations (or exit)\n";
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
                //std::cout << input << '\n';
                //std::cout << fraction1 << ' ' << fraction2 << ' ' << operation << '\n';
                operations(fraction1, fraction2, operation);
                break;
            case 0:
                std::cout << "good bye\n";
                return 0;
        }
        break;
    }
}

