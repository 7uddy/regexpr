#pragma once
#include <stack>
#include <fstream>
#include <vector>
#include <string>


int priority(const char& oper);
bool verifyAlNum(const char& character);
void eraseSpace(std::string& expression);

bool checkInput(char previousBracket, char rearBracket);
bool reduceBrackets(char previousBracket, char rearBracket);
bool existsAtLeastOneCharacter(const std::string& expression);
bool isValid(const std::string& expression);



void insertDot(std::string& expression);
void modifyExpression(std::string& expression);

std::string ReadRegularExpressionFromFile(const std::string& filename);
std::vector <char> GetPolishForm(std::string);