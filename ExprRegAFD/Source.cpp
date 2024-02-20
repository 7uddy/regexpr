#include "FiniteAutomaton.h";
#include "PolishForm.h"

import <iostream>;
import <fstream>;
import <vector>;
import <stack>;

static DeterministicFiniteAutomaton GetAFDFromPolishForm(const std::vector<char>& polishForm)
{
	std::stack<FiniteAutomaton> SA;
	FiniteAutomaton a, b;
	for (size_t index = 0u; index < polishForm.size(); ++index)
	{
		switch (polishForm[index])
		{
		case '.':
			b = SA.top();
			SA.pop();
			a = SA.top();
			SA.pop();
			SA.push(FiniteAutomaton::ConcatenateTwoAutomatons(a, b));
			break;
		case '|':
			b = SA.top();
			SA.pop();
			a = SA.top();
			SA.pop();
			SA.push(FiniteAutomaton::OrTwoAutomatons(a, b));
			break;
		case '*':
			a = SA.top();
			SA.pop();
			SA.push(FiniteAutomaton::Stelare(a));
			break;
		default: //for characters a, b, etc
			SA.push(FiniteAutomaton::GetAutomatonWithCharacter(polishForm[index]));
			break;
		}
	}
	FiniteAutomaton AFNWithLambdaTransitions = SA.top();
	SA.pop();
	return FiniteAutomaton::AFNtoAFD(AFNWithLambdaTransitions);
}

static void showMenu(const std::string& regularExpression)
{
	std::vector<char> polishForm{ GetPolishForm(regularExpression) };
	DeterministicFiniteAutomaton deterministicFiniteAutomaton{ GetAFDFromPolishForm(polishForm) };

	int choice{};
	std::string inputWord{};

	std::ofstream fout;
	std::string filename{};
	do
	{
		std::cout << "\nMENU:\n";
		std::cout << "1. Print automaton in console and in a specified file name.\n";
		std::cout << "2. Print regular expression from file.\n";
		std::cout << "3. Check a given word in automaton.\n";
		std::cout << "0. Exit menu.\n";
		std::cout << "\nYOUR CHOICE: ";
		std::cin >> choice;
		switch (choice)
		{
		case 1:
			std::cout << "PLEASE ENTER FILENAME: ";
			std::cin >> filename;
			deterministicFiniteAutomaton.PrintAutomaton(std::cout);
			try 
			{
				fout.open(filename);
				fout << deterministicFiniteAutomaton;
				fout.close();
			}
			catch (...)
			{
				std::cout << "EROARE LA MANIPULAREA FISIERULUI.";
			}
			break;
		case 2:
			std::cout << "\nREGULAR FORM IS:\n" << regularExpression << "\n";
			break;
		case 3:
			std::cout << "\nPLEASE INPUT WORD: ";
			std::cin >> inputWord;
			std::cout << "RESULT:\n";
			std::cout << std::boolalpha << deterministicFiniteAutomaton.CheckWord(inputWord) << "\n";
			break;
		case 0:
			std::cout << "\nYOU HAVE CHOSEN TO EXIT MENU.\n";
			break;
		default:
			std::cout << "\nINVALID CHOICE. PLEASE ENTER ANOTHER VALUE.\n";
			break;
		}

	} while (choice != 0);
}


int main()
{
	std::string regularExpression{ ReadRegularExpressionFromFile("polishForm.txt")};

	if (!isValid(regularExpression))
	{
		std::cout << "Regular expression from file is not valid.";
		return 1;
	}
	showMenu(regularExpression);
	return 0;
}