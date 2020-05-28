#pragma once
#include <string>
#include <set>
#include "AutomatHeader.h"

struct MooreState
{
	std::string name;
	std::string outputSignal;

	bool operator==( MooreState const& state ) const {
		return this->name == state.name && this->outputSignal == state.outputSignal;
	}

	bool operator< ( MooreState const& state ) const {
		return this->name < state.name
			|| (this->name == state.name && this->outputSignal < state.outputSignal);
	}
};

struct MooreTransition
{
	MooreState currentState;
	MooreState nextState;
	std::string inputSignal;

	bool operator==( MooreTransition const& transition ) const {
		return this->currentState == transition.currentState
			&& this->nextState == transition.nextState
			&& this->inputSignal == transition.inputSignal;
	}

	bool operator< ( MooreTransition const& transition ) const {
		return this->currentState < transition.currentState
			|| (this->currentState == transition.currentState && this->nextState < transition.nextState)
			|| (this->currentState == transition.currentState && this->nextState == transition.nextState
				 && this->inputSignal < transition.inputSignal);
	}
};

class CAutomatMoore
{
public:
	CAutomatMoore();
	~CAutomatMoore();
	void AddState( std::string stateName, std::string outputSignal );
	void AddState( MooreState state );
	void AddTransition( MooreState currentState, MooreState nextState, std::string inputSignal );
	void AddTransition( MooreTransition transition );
	std::set<MooreState> GetStates();
	MooreState GetState( std::string stateName );
	std::set<MooreTransition> GetTransitions();
	std::set<MooreTransition> GetTransitions( MooreState currentState );
	MooreTransition GetTransition( MooreState currentState, std::string inputSignal );
	bool ContainsState( MooreState state );
	bool ContainsTransition( MooreTransition transition );
	std::set<std::string> GetInputSignals();
	void Save( std::string fileName, SaveFormat format );
private:
	std::set<MooreState> m_states;
	std::set<MooreTransition> m_transitions;

	void SaveInDot( std::string fileName );
	void SaveInTxt( std::string fileName );
};
