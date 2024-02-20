#include "PolishForm.h"

int priority(const char& oper)
{
    if (oper == '|')
        return 1;
    if (oper == '.')
        return 2;
    if (oper == '*')
        return 3;
    if (oper == '(')
        return 0;
}

bool verifyAlNum(const char& character)
{
    if ((character >= 'a' && character <= 'z') || (character >= '0' && character <= '9'))
        return true;
    return false;
}
bool checkInput(char previousBracket, char rearBracket)
{
    if (previousBracket < rearBracket)
        return false;
    return true;
}

bool reduceBrackets(char previousBracket, char rearBracket)
{
    if (previousBracket - rearBracket == -1)
        return true;
    return false;
}

bool existsAtLeastOneCharacter(const std::string& expression)
{
    for (const auto& caracter : expression)
        if (verifyAlNum(caracter))
            return true;
    return false;
}

bool isValid(const std::string& expression)
{
    /*if (expression.size() == 1)
        if (verifyAlNum(expression[0]))
            return true;*/
    //verifica daca expresia contine caractere
    if (!existsAtLeastOneCharacter(expression))
        return false;
    //verifica daca toate () se inchid
    if (expression.size() < 2) return false;
    std::stack<char> brackets;
    for (char character : expression)
    {
        if (character == '(')
        {
            if (brackets.empty() == true)
                brackets.push(character);
            else
            {
                if (checkInput(brackets.top(), character) == true)
                    brackets.push(character);
                else
                    return false;
            }
        }
        else if (character == ')')
        {
            if (brackets.empty() == true)
                return false;
            if (reduceBrackets(brackets.top(), character) == true)
                brackets.pop();
            else
                return false;
        }
    }
    if (brackets.empty())
        return true;
    return false;
}

void eraseSpace(std::string& expression)
{
    for (int i = 0; i < expression.length(); i++)
    {
        while (expression[i] == ' ')
        {
            expression.erase(i, 1);
        }
    }
}



void insertDot(std::string& expression)
{
    for (int i = 0; i < expression.length(); i++)
    {
        if (verifyAlNum(expression[i]) && (verifyAlNum(expression[i + 1]) || expression[i + 1] == '(') && i + 1 < expression.length())
        {
            expression.insert(i + 1, 1, '.');
            continue;
        }
        if (expression[i] == ')' && (verifyAlNum(expression[i + 1]) || expression[i+1]=='('))
        {
            expression.insert(i + 1, 1, '.');
            continue;
        }
        if (expression[i] == '*' && verifyAlNum(expression[i + 1]) && i + 1 < expression.length())
        {
            expression.insert(i + 1, 1, '.');
            continue;
        }

    }
}

void modifyExpression(std::string& expression)
{
    eraseSpace(expression);
    insertDot(expression);
}

std::string ReadRegularExpressionFromFile(const std::string& filename)
{
    std::string expression{};
    std::ifstream in{ filename };
    if (in.is_open())
    {
        std::getline(in, expression);
        in.close();
    }
    else
    {
        throw std::exception{ "The file could not be opened" };
    }
    return expression;
}

std::vector<char> GetPolishForm(std::string expression)
{
    std::vector<char> polishForm;
    std::stack<char> operators;
    modifyExpression(expression);
    for (char character : expression)
    {
        if (verifyAlNum(character))
        {
            polishForm.push_back(character);
        }
        else if (character == '(')
        {
            operators.push(character);
        }
        else if (character == ')')
        {
            while (!operators.empty() && operators.top() != '(')
            {
                polishForm.push_back(operators.top());
                operators.pop();
            }
            operators.pop();
        }
        else if (character == '.' || character == '|' || character == '*')
        {
            while (!operators.empty() && priority(character) <= priority(operators.top()))
            {
                polishForm.push_back(operators.top());
                operators.pop();
            }
            operators.push(character);
        }
        else
        {
            polishForm.clear();
            throw std::exception("Expression is invalid\n");
        }
    }
    while (!operators.empty())
    {
        polishForm.push_back(operators.top());
        operators.pop();
    }

    return polishForm;
}


