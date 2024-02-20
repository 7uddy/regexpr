
#pragma once
import <iostream>;
import <unordered_map>;
import <map>;
import <string>;
import <algorithm>;

class DeterministicFiniteAutomaton
{
public:
	DeterministicFiniteAutomaton() = default;

	void SetPossibleStates(const std::vector<char>&) noexcept;
	void SetAlphabet(const std::vector<char>&) noexcept;
	void SetInitialState(const char&) noexcept;
	void SetFinalStates(bool) noexcept;
	void SetFinalStates(const std::vector<char>& argFinalStates) noexcept;
	void SetFunctions(const std::map<char, std::unordered_map<char, char>>&) noexcept;
	void SetLambda(const char&) noexcept;

	std::vector<char> GetFinalStates() const noexcept;
	char GetInitialState() const noexcept;

	void PrintAutomaton(std::ostream&) const noexcept;

	bool CheckWord(const std::string&) const noexcept;
	bool VerifyAutomaton() const noexcept;

	friend std::ostream& operator << (std::ostream&, const DeterministicFiniteAutomaton&);
private:

	bool CheckValidWord(const char&, const std::string&) const noexcept;
	bool WordHasValidCharacters(const std::string& word) const noexcept;
	bool InitialToFinalRoute(std::unordered_map<char, bool>& visitedStates, char state) const noexcept;
	bool IsStateInPossibleStates(const char& state) const noexcept;
	bool IsStateMadeWithAlphabet(const char& state) const noexcept;
	bool CheckStatesExistence()const noexcept;

private:
	static char kLambda;

private:
	std::vector<char> m_possibleStates;
	std::vector<char> m_alphabet;
	char m_initialState;
	std::vector<char> m_finalStates;

	//A cu a se poate duce in A
	//A cu b se poate duce in B
	std::map<char, std::unordered_map<char,char>> m_transitions;

};

