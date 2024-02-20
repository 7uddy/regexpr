#include "FiniteAutomaton.h"
#include <unordered_set>

char FiniteAutomaton::kLambda{ '@' };
int FiniteAutomaton::kCounter = 0;
char FiniteAutomaton::kFirstCharacter = 'A';

FiniteAutomaton FiniteAutomaton::GetAutomatonWithCharacter(char character)
{
	FiniteAutomaton automaton;
	//Calculate states for automatom
	char startState = kFirstCharacter + kCounter;
	char finalState = kFirstCharacter + kCounter + 1;
	//Initialize automatom members
	automaton.m_initialState = (startState);

	automaton.m_alphabet.emplace_back(character);
	automaton.m_finalStates.emplace_back(finalState);

	automaton.m_possibleStates.emplace_back(startState);
	automaton.m_possibleStates.emplace_back(finalState);

	//Update Functions
	std::vector<char> endState{ finalState };
	std::unordered_map<char, std::vector<char>> func;
	func.emplace(std::make_pair(character, endState));
	automaton.m_transitions.insert(std::make_pair(startState, func));

	//Update kCounter for future automatons
	kCounter += 2;
	return automaton;

}

FiniteAutomaton FiniteAutomaton::ConcatenateTwoAutomatons(const FiniteAutomaton& A, const FiniteAutomaton& B)
{
	//FAST
	if (A.m_alphabet.empty())
		return B;
	if (B.m_alphabet.empty())
		return A;
	//ERROR MANAGEMENT
	if (A.m_alphabet.empty() && B.m_alphabet.empty())
		throw std::exception("BOTH AUTOMATONS ARE EMPTY. UNABLE TO CONCATENATE");
	//GET AUTOMATON
	FiniteAutomaton result = A;
	std::copy_if(B.m_alphabet.begin(), B.m_alphabet.end(), std::back_inserter(result.m_alphabet),
		[&result](char character) {
			return std::find(result.m_alphabet.begin(), result.m_alphabet.end(), character) == result.m_alphabet.end();
		});
	std::copy_if(B.m_possibleStates.begin(), B.m_possibleStates.end(), std::back_inserter(result.m_possibleStates),
		[&B](char state) {
			return state != B.GetInitialState();
		});

	std::unordered_map<char, std::vector<char>> func = B.m_transitions.at(B.m_initialState);
	for (const auto& element : B.m_transitions)
	{
		if (element.first == B.m_initialState)
			result.m_transitions.insert(std::make_pair(A.m_finalStates[0], B.m_transitions.at(B.m_initialState)));
		else
			result.m_transitions.emplace(element);

	}
	std::copy(B.m_finalStates.begin(), B.m_finalStates.end(), result.m_finalStates.begin());

	return result;

}

FiniteAutomaton FiniteAutomaton::Stelare(const FiniteAutomaton& A)
{
	if (A.m_alphabet.empty())
		throw std::exception("AUTOMATON IS EMPTY. UNABLE TO STELARE.");
	FiniteAutomaton result = A;
	char startState = kFirstCharacter + kCounter;
	char finalState = kFirstCharacter + kCounter + 1;

	std::unordered_map<char, std::vector<char>> emptyMap;
	std::vector<char> emptyVector;

	result.m_transitions.insert(std::make_pair(startState, emptyMap));
	result.m_transitions.at(startState).insert(std::make_pair(kLambda, emptyVector));
	result.m_transitions.insert(std::make_pair(A.m_finalStates[0], emptyMap));
	result.m_transitions.at(A.m_finalStates[0]).insert(std::make_pair(kLambda, emptyVector));

	result.m_transitions[startState].at(kLambda).emplace_back(A.m_initialState);
	result.m_transitions[startState].at(kLambda).emplace_back(finalState);
	result.m_transitions[A.m_finalStates[0]].at(kLambda).emplace_back(A.m_initialState);
	result.m_transitions[A.m_finalStates[0]].at(kLambda).emplace_back(finalState);

	result.m_possibleStates.emplace_back(startState);
	result.m_possibleStates.emplace_back(finalState);
	result.m_initialState = startState;
	result.m_finalStates[0] = finalState;
	kCounter += 2;
	return result;
}

FiniteAutomaton FiniteAutomaton::OrTwoAutomatons(const FiniteAutomaton& A, const FiniteAutomaton& B)
{
	if (A.m_alphabet.empty() || B.m_alphabet.empty())
		throw std::exception("AUTOMATON IS EMPTY. UNABLE TO OR.");
	FiniteAutomaton result = A;

	char startState = kFirstCharacter + kCounter;
	char finalState = kFirstCharacter + kCounter + 1;

	std::unordered_map<char, std::vector<char>> emptyMap;
	std::vector<char> emptyVector;

	result.m_transitions.insert(std::make_pair(startState, emptyMap));
	result.m_transitions.at(startState).insert(std::make_pair(kLambda, emptyVector));
	result.m_transitions.insert(std::make_pair(A.m_finalStates[0], emptyMap));
	result.m_transitions.at(A.m_finalStates[0]).insert(std::make_pair(kLambda, emptyVector));
	result.m_transitions.insert(std::make_pair(B.m_finalStates[0], emptyMap));
	result.m_transitions.at(B.m_finalStates[0]).insert(std::make_pair(kLambda, emptyVector));

	result.m_transitions[startState].at(kLambda).emplace_back(A.m_initialState);
	result.m_transitions[startState].at(kLambda).emplace_back(B.m_initialState);
	result.m_transitions[A.m_finalStates[0]].at(kLambda).emplace_back(finalState);
	result.m_transitions[B.m_finalStates[0]].at(kLambda).emplace_back(finalState);

	for (const auto& element : B.m_transitions)
	{
		result.m_transitions.emplace(element);
	}

	for (const auto& element : B.m_possibleStates)
	{
		result.m_possibleStates.emplace_back(element);
	}

	std::copy_if(B.m_alphabet.begin(), B.m_alphabet.end(), std::back_inserter(result.m_alphabet),
		[&result](char character) {
			return std::find(result.m_alphabet.begin(), result.m_alphabet.end(), character) == result.m_alphabet.end();
		});

	result.m_possibleStates.emplace_back(startState);
	result.m_possibleStates.emplace_back(finalState);
	result.m_initialState = startState;
	result.m_finalStates[0] = finalState;
	kCounter += 2;
	return result;
}
std::vector<char> FiniteAutomaton::GenerateStates(const std::vector<char>& states,const char& character)
{
	std::vector<char>auxStates;
	for (const auto& state : states)
	{
		std::vector<char>deltaStates(Delta(state, character));
		std::vector<char>lambdaStates;
		std::vector<char>auxLambda;

		for (const auto& deltaState : deltaStates)
		{
			auxLambda.clear();
			LambdaClosing(deltaState, auxLambda);
			std::copy_if(auxLambda.begin(), auxLambda.end(), std::back_inserter(lambdaStates),
				[&lambdaStates](char character) {
					return std::find(lambdaStates.begin(), lambdaStates.end(), character) == lambdaStates.end();
				});
		}

		std::sort(lambdaStates.begin(), lambdaStates.end());
		std::copy_if(lambdaStates.begin(), lambdaStates.end(), std::back_inserter(auxStates),
			[&auxStates](char character) {
				return std::find(auxStates.begin(), auxStates.end(), character) == auxStates.end();
			});

	}
	return auxStates;
}
DeterministicFiniteAutomaton FiniteAutomaton::AFNtoAFD(FiniteAutomaton& AFNautomaton)
{
	std::unordered_map<char, std::vector<char>>oldStates;
	std::map<char, std::unordered_map<char, char>> newTransitions;
	std::vector<char> auxStates;

	AFNautomaton.LambdaClosing(AFNautomaton.m_initialState, auxStates);
	std::sort(auxStates.begin(), auxStates.end());
	char oldBigState = 'A';
	AFNautomaton.m_initialState = oldBigState;
	oldStates[oldBigState] = auxStates;
	char newBigState = 'A' - 1;

	for (const auto& it : oldStates)
	{
		newBigState++;
		for (const auto& character : AFNautomaton.m_alphabet)
		{
			auxStates = AFNautomaton.GenerateStates(it.second, character);
			if (!auxStates.empty())
			{
				char wasFound = ' ';
				for (const auto& pair : oldStates)
					if (std::equal(pair.second.begin(), pair.second.end(), auxStates.begin(), auxStates.end()))
					{
						wasFound = pair.first;
					}
				if (wasFound == ' ')
				{
					oldBigState++;
					oldStates.emplace(oldBigState, auxStates);
					newTransitions[newBigState][character]=(oldBigState);
				}
				else
				{
					newTransitions[newBigState][character]=(wasFound);
				}
			}
		}
	}

	// Initializations for DeterministicFiniteAutomaton members
	DeterministicFiniteAutomaton AFDautomaton;
	std::vector<char>possibleStates;
	std::vector<char>finalStates;
	for (const auto& possible : oldStates)
	{
		possibleStates.emplace_back(possible.first);
		for (const auto& afdFinal : possible.second)
		{
			for (const auto& afnFinal : AFNautomaton.m_finalStates)
			{
				if (afdFinal == afnFinal) finalStates.emplace_back(possible.first);
			}
		}
	}
	
	AFDautomaton.SetInitialState('A');
	AFDautomaton.SetFinalStates(finalStates);
	AFDautomaton.SetPossibleStates(possibleStates);
	AFDautomaton.SetAlphabet(AFNautomaton.m_alphabet);
	AFDautomaton.SetFunctions(newTransitions);
	//std::cout << AFDautomaton;

	return AFDautomaton;
}

void FiniteAutomaton::SetPossibleStates(const std::vector<char>& characters) noexcept
{
	m_possibleStates = characters;
	int i = 0;
	while (i < characters.size())
	{
		char newCharacter = characters[i] + 1;
		auto found = std::find(m_possibleStates.begin(), m_possibleStates.end(), newCharacter);
		if (found == m_possibleStates.end())
		{
			m_possibleStates.emplace_back(newCharacter);
			return;
		}
		i++;
	}
}

void FiniteAutomaton::SetAlphabet(const std::vector<char>& characters) noexcept
{
	m_alphabet = characters;
}


void FiniteAutomaton::SetInitialState(const char& character) noexcept
{
	m_initialState = character;
}

void FiniteAutomaton::SetFinalStates(bool grammarContainsLambda = false) noexcept
{
	m_finalStates.emplace_back(m_possibleStates[m_possibleStates.size() - 1]);
	if (grammarContainsLambda)
		m_finalStates.emplace_back(m_initialState);
}

void FiniteAutomaton::SetFunctions(const std::map<char, std::unordered_map<char, std::vector<char>>>& function) noexcept
{
	m_transitions = function;
}

void FiniteAutomaton::SetLambda(const char& character) noexcept
{
	kLambda = character;
}


std::vector<char> FiniteAutomaton::GetFinalStates() const noexcept
{
	return m_finalStates;
}

char FiniteAutomaton::GetInitialState() const noexcept
{
	return m_initialState;
}

void FiniteAutomaton::PrintAutomaton(std::ostream& os) const noexcept
{
	os << "AUTOMATON:\n";
	os << *this;
}

bool FiniteAutomaton::WordHasValidCharacters(const std::string& word) const noexcept
{
	for (auto& letter : word)
	{
		auto found = std::find(m_alphabet.begin(), m_alphabet.end(), letter);
		if (found == m_alphabet.end())
			return false;
	}
	return true;
}

bool FiniteAutomaton::CheckWord(const std::string& word) const noexcept
{
	if (word.size() == 1)
		if (word[0] == kLambda)
			return std::find(m_finalStates.begin(), m_finalStates.end(), m_initialState) != m_finalStates.end();
	if (!WordHasValidCharacters(word))
		return false;
	return CheckValidWord(m_initialState, word);
}

bool FiniteAutomaton::CheckValidWord(const char& currentState, const std::string& currentWord) const noexcept
{
	//std::cout << currentState << " with ";
	if (currentWord.length() == 0 || currentWord[0] == kLambda)
	{
		if (m_finalStates[0] == currentState)
			return true;
		if (m_finalStates.size() == 2)
			if (m_finalStates[1] == currentState)
				return true;
		return false;
	}
	if (!m_transitions.contains(currentState))
		return false;
	for (const auto& function : m_transitions.at(currentState))
	{
		//std::cout << function.first << " can go in ";
		for (const auto& elements : function.second)
		{
			//std::cout << elements << " ";
			bool found = CheckValidWord(elements, currentWord.substr(1, currentWord.length() - 1));
			if (found) return true;
		}
		//std::cout << "\n";
	}
	return false;
}

bool FiniteAutomaton::InitialToFinalRoute(std::unordered_map<char, bool>& visitedStates, char state = '-') const noexcept
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
		std::unordered_map<char, std::vector<char>> functie = m_transitions.at(state);
		for (const auto& dreapta : functie)
		{
			for (const auto& nextState : dreapta.second)
			{
				if (!visitedStates[nextState])
				{
					if (InitialToFinalRoute(visitedStates, nextState))
						return true;
				}
			}
		}
	}
	return false;
}

bool FiniteAutomaton::IsStateInPossibleStates(const char& state) const noexcept {
	for (const auto& possibleState : m_possibleStates)
	{
		if (possibleState == state) return true;
	}
	return false;
}

bool FiniteAutomaton::IsStateMadeWithAlphabet(const char& state) const noexcept
{
	for (const auto& letter : m_alphabet)
	{
		if (letter == state) return false;
	}
	return true;
}

bool FiniteAutomaton::CheckStatesExistence() const noexcept
{
	return !m_finalStates.empty() && m_initialState != '\0';
}

std::vector<char> FiniteAutomaton::Delta(const char& state, const char& character) const
{
	if (!IsStateInPossibleStates(state))
		throw std::exception("Can't apply delta. State doesn't exist.");
	if (m_transitions.find(state) != m_transitions.end())
	{
		for (const auto& roads : m_transitions.at(state))
		{
			if (roads.first == character)
			{
				return roads.second;
			}
		}
	}
	//return std::vector<char>{};
}

void FiniteAutomaton::LambdaClosing(const char& state, std::vector<char>& visitedStates) const
{
	if (!IsStateInPossibleStates(state))
		throw std::exception("Can't apply lambda. State doesn't exist.");
	visitedStates.push_back(state);
	if (m_transitions.find(state) != m_transitions.end())
	{
		for (const auto& roads : m_transitions.at(state))
		{
			if (roads.first == kLambda)
			{
				for (const auto& toVisitState : roads.second)
				{
					LambdaClosing(toVisitState, visitedStates);
				}

			}
		}
	}
}


bool FiniteAutomaton::VerifyAutomaton() const noexcept
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

bool FiniteAutomaton::IsDeterministic() const noexcept
{
	for (const auto& stateFunction : m_transitions)
	{
		std::unordered_map<char, std::vector<char>> possibleScenarios = stateFunction.second;
		for (const auto& nextState : possibleScenarios)
		{
			if (nextState.second.size() > 1) return false;
		}
	}
	return true;
}

std::ostream& operator<<(std::ostream& out, const FiniteAutomaton& automaton)
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
			for (const auto& finala : dreapta.second)
			{
				out << finala << " ";
			}
			out << "}\n";
		}
	}
	out << "Final states = { ";
	for (const auto& element : automaton.m_finalStates)
		out << element << " ";
	out << "}\n";
	return out;
}
