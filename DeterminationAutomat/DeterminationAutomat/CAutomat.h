#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <map>

struct State
{
	std::string name;

	bool operator==( State const& state ) const {
		return this->name == state.name;
	}
};

struct Transition
{
	State currentState;
	State nextState;
	std::string inputSignal;

	bool operator==( Transition const& transition ) const {
		return this->currentState == transition.currentState
			&& this->nextState == transition.nextState
			&& this->inputSignal == transition.inputSignal;
	}
};

struct AuxiliaryTransition
{
	std::vector<State> currentStates;
	std::vector<State> nextStates;
	std::string inputSignal;
};

class CAutomat
{
public:
	CAutomat();
	CAutomat( std::ifstream& stream );
	~CAutomat();
	void AddState( State& state);
	void AddTransition( Transition& transition );
	std::vector<Transition> GetTransitions( State& currentState );
	std::vector<std::string> GetInputSignals();
	std::vector<Transition> GetTransitions( State& currentState, std::string inputSignal );
	void SaveInDot( std::string fileName );
	void Determine();
private:
	std::vector<State> m_states;
	std::vector<Transition> m_transitions;

	std::vector<Transition> GetTransitions( std::vector<State>& states, std::string inputSignal );
	void ShakerSortStates( std::vector<State>& states );
	void Update( std::vector<AuxiliaryTransition>& newTransitions, std::vector<std::vector<State>>& newStates );
};

