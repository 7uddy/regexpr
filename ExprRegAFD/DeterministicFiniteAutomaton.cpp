#include "DeterministicFiniteAutomaton.h"
#include <unordered_set>

char DeterministicFiniteAutomaton::kLambda{ '@' };

void DeterministicFiniteAutomaton::SetPossibleStates(const std::vector<char>& characters) noexcept
{
	m_possibleStates = characters;
}

void DeterministicFiniteAutomaton::SetAlphabet(const std::vector<char>& characters) noexcept
{
	m_alphabet = characters;
}


void DeterministicFiniteAutomaton::SetInitialState(const char& character) noexcept
{
	m_initialState = character;
}

void DeterministicFiniteAutomaton::SetFinalStates(bool grammarContainsLambda = false) noexcept
{
	m_finalStates.emplace_back(m_possibleStates[m_possibleStates.size() - 1]);
	if (grammarContainsLambda)
		m_finalStates.emplace_back(m_initialState);
}
void DeterministicFiniteAutomaton::SetFinalStates(const std::vector<char>& argFinalStates) noexcept
{
	m_finalStates = argFinalStates;
}

void DeterministicFiniteAutomaton::SetFunctions(const std::map<char, std::unordered_map<char, char>>& function) noexcept
{
	m_transitions = function;
}

void DeterministicFiniteAutomaton::SetLambda(const char& character) noexcept
{
	kLambda = character;
}


std::vector<char> DeterministicFiniteAutomaton::GetFinalStates() const noexcept
{
	return m_finalStates;
}

char DeterministicFiniteAutomaton::GetInitialState() const noexcept
{
	return m_initialState;
}

void DeterministicFiniteAutomaton::PrintAutomaton(std::ostream& os) const noexcept
{
	os << "AUTOMATON:\n";
	os << *this;
}

bool DeterministicFiniteAutomaton::WordHasValidCharacters(const std::string& word) const noexcept
{
	for (auto& letter : word)
	{
		auto found = std::find(m_alphabet.begin(), m_alphabet.end(), letter);
		if (found == m_alphabet.end())
			return false;
	}
	return true;
}

bool DeterministicFiniteAutomaton::CheckWord(const std::string& word) const noexcept
{
	if (word.size() == 1)
		if (word[0] == kLambda)
			return std::find(m_finalStates.begin(), m_finalStates.end(), m_initialState) != m_finalStates.end();
	if (!WordHasValidCharacters(word))
		return false;
	return CheckValidWord(m_initialState, word);
}

bool DeterministicFiniteAutomaton::CheckValidWord(const char& currentState, const std::string& currentWord) const noexcept
{
	if (currentWord.length() == 0 || currentWord[0] == kLambda)
	{
		if (m_finalStates[0] == currentState)
			return true;
		if (m_finalStates.size() == 2)
			if (m_finalStates[1] == currentState)
				return true;
		return false;
	}
	//check if from currentstate you can go somewhere
	if (!m_transitions.contains(currentState))
		return false;
	for (const auto& function : m_transitions.at(currentState))
	{
		if (function.first != currentWord[0])
			continue;
		bool found = CheckValidWord(function.second, currentWord.substr(1, currentWord.length() - 1));
		if (found) return true;
	}
	return false;
}

bool DeterministicFiniteAutomaton::InitialToFinalRoute(std::unordered_map<char, bool>& visitedStates, char state = '-') const noexcept
{
	if (state == '-')
	{
		state = m_initialState;
	}
	visitedStates[state] = true;
	if (std::find(m_finalStates.begin(), m_finalStates.end(), state) != m_finalStates.end())
	{
		return true;
	}
	else {
		std::unordered_map<char, char> functie = m_transitions.at(state);
		for (const auto& dreapta : functie)
		{

			if (!visitedStates[dreapta.second])
			{
				if (InitialToFinalRoute(visitedStates, dreapta.second))
					return true;
			}

		}
	}
	return false;
}

bool DeterministicFiniteAutomaton::IsStateInPossibleStates(const char& state) const noexcept {
	for (const auto& possibleState : m_possibleStates)
	{
		if (possibleState == state) return true;
	}
	return false;
}

bool DeterministicFiniteAutomaton::IsStateMadeWithAlphabet(const char& state) const noexcept
{
	for (const auto& letter : m_alphabet)
	{
		if (letter == state) return false;
	}
	return true;
}

bool DeterministicFiniteAutomaton::CheckStatesExistence() const noexcept
{
	return !m_finalStates.empty() && m_initialState != '\0';
}

bool DeterministicFiniteAutomaton::VerifyAutomaton() const noexcept
{
	std::unordered_map<char, bool> visitedStates;
	if (m_finalStates.empty())
		return false;
	if (!InitialToFinalRoute(visitedStates))
		return false;
	if (!IsStateInPossibleStates(m_initialState))
		return false;
	for (const auto& finalState : m_finalStates)
	{
		if (!IsStateInPossibleStates(finalState))
			return false;
		if (!IsStateMadeWithAlphabet(finalState))
			return false;
	}
	if (!CheckStatesExistence())
		return false;

	return true;
}

std::ostream& operator<<(std::ostream& out, const DeterministicFiniteAutomaton& automaton)
{
	out << "POSSIBLE STATES = { ";
	for (const auto& element : automaton.m_possibleStates)
		out << element << " ";
	out << "}\nALPHABET = { ";
	for (const auto& element : automaton.m_alphabet)
		out << element << " ";
	out << "}\nInitial state: " << automaton.m_initialState;
	out << "\nLambda: " << automaton.kLambda;
	out << "\nFunctions:\n";
	for (const auto& functie : automaton.m_transitions)
	{
		for (const auto& dreapta : functie.second)
		{
			out << "(" << functie.first << ", ";
			out << dreapta.first << ") = { ";
			out << dreapta.second;
			out << "}\n";
		}
	}
	out << "Final states = { ";
	for (const auto& element : automaton.m_finalStates)
		out << element << " ";
	out << "}\n";
	return out;
}
