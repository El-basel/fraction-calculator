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

void fraction_to_ints(std::string fraction,int& numerator,int& denominator)
{
    //added a space so it can be easily known if the loop reached the end or not
    fraction += ' ';
    // collects the numbers to add it to either numerator or denominator
    std::string number_string = "";
    // denominator equals 1 so if the number is an int it will do the operations right
    numerator = 0, denominator = 1;
    // n is a vriable used as a refrence if we added a numerator or not and number_int is where we convert strings to integers 
    int n = 1, number_int = 0;
    for (int i = 0; i < fraction.size(); i++)
    {
        // if the loop stopped on a '/' it means the numerator ended
        // but if n is 1 and it found a ' ' then it means the number is an integer 
        if (fraction[i] == '/' || (n == 1 && fraction[i] == ' '))
        {
            number_int = stoi(number_string);
            numerator += number_int;
            number_string = "";
            ++n;
        }
        // if n is 2 it means we added a numerator and we need to add a denominator
        else if ( fraction[i] == ' ' && n == 2)
        {
            number_int = stoi(number_string);
            denominator = number_int;
        }
        else number_string += fraction[i];
    }
    // used to indentify an error
    if (denominator == 0) numerator = denominator = -1;
}
void operations(std::string& fraction1, std::string& fraction2, std::string& operation)
{
    int ans_numerator{ 0 }, ans_denominator{ 0 };
    int numerator1{ 0 }, denominator1{ 0 }, numerator2{ 0 }, denominator2{ 0 };
    fraction_to_ints(fraction1 ,numerator1, denominator1);
    fraction_to_ints(fraction2, numerator2, denominator2);
    // an error found in the fraction to int function
    if (denominator1 == -1 || denominator2 == -1)
    {
        std::cout << "\adenominator cant equal zero";
        return;
    }
    //operations are done as we do it manually
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