#pragma once
#include "DeterministicFiniteAutomaton.h"
import <iostream>;
import <unordered_map>;
import <map>;
import <string>;
import <algorithm>;

class FiniteAutomaton
{
public:
	FiniteAutomaton() = default;

	static FiniteAutomaton GetAutomatonWithCharacter(char character);
	static FiniteAutomaton ConcatenateTwoAutomatons(const FiniteAutomaton&, const FiniteAutomaton&);
	static FiniteAutomaton Stelare(const FiniteAutomaton&);
	static FiniteAutomaton OrTwoAutomatons(const FiniteAutomaton&, const FiniteAutomaton&);

	static DeterministicFiniteAutomaton AFNtoAFD( FiniteAutomaton& AFNautomaton);
	std::vector<char> GenerateStates(const std::vector<char>& states,const char& character);

	void SetPossibleStates(const std::vector<char>&) noexcept;
	void SetAlphabet(const std::vector<char>&) noexcept;
	void SetInitialState(const char&) noexcept;
	void SetFinalStates(bool) noexcept;
	void SetFunctions(const std::map<char, std::unordered_map<char, std::vector<char>>>&) noexcept;
	void SetLambda(const char&) noexcept;

	std::vector<char> GetFinalStates() const noexcept;
	char GetInitialState() const noexcept;

	void PrintAutomaton(std::ostream&) const noexcept;

	bool CheckWord(const std::string&) const noexcept;
	bool VerifyAutomaton() const noexcept;
	bool IsDeterministic() const noexcept;

	friend std::ostream& operator << (std::ostream&, const FiniteAutomaton&);

	std::vector<char> Delta(const char& state, const char& character) const;
	void LambdaClosing(const char& state,std::vector<char>&visitedState) const;
private:

	bool CheckValidWord(const char&, const std::string&) const noexcept;
	bool WordHasValidCharacters(const std::string& word) const noexcept;
	bool InitialToFinalRoute(std::unordered_map<char, bool>& visitedStates, char state) const noexcept;
	bool IsStateInPossibleStates(const char& state) const noexcept;
	bool IsStateMadeWithAlphabet(const char& state) const noexcept;
	bool CheckStatesExistence()const noexcept;
	
private:
	static char kLambda;
	static char kFirstCharacter;
	static int kCounter;

private:
	std::vector<char> m_possibleStates;
	std::vector<char> m_alphabet;
	char m_initialState;
	std::vector<char> m_finalStates;

	//A cu a se poate duce in A,B,C
	//A cu b se poate duce in B, C
	std::map<char, std::unordered_map<char, std::vector<char>>> m_transitions;

};

