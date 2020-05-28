#pragma once
#include <string>
#include <set>
#include "AutomatHeader.h"

struct MealyState
{
	std::string name;

	bool operator==( MealyState const& state ) const {
		return this->name == state.name;
	}

	bool operator< ( MealyState const& state ) const {
		return this->name < state.name;
	}
};

struct MealyTransition
{
	std::string currentState;
	std::string nextState;
	std::string inputSignal;
	std::string outputSignal;

	bool operator==( MealyTransition const& transition ) const {
		return this->currentState == transition.currentState
			&& this->nextState == transition.nextState
			&& this->inputSignal == transition.inputSignal
			&& this->outputSignal == transition.outputSignal;
	}

	bool operator< (MealyTransition const& transition) const {
		return this->currentState < transition.currentState
			|| (this->currentState == transition.currentState && this->nextState < transition.nextState)
			|| (this->currentState == transition.currentState && this->nextState == transition.nextState
				 && this->inputSignal < transition.inputSignal)
			|| (this->currentState == transition.currentState && this->nextState == transition.nextState
				 && this->inputSignal == transition.inputSignal && this->outputSignal < transition.outputSignal);
	}
};

class CAutomatMealy
{
public:
	CAutomatMealy();
	~CAutomatMealy();
	void AddState( std::string stateName );
	void AddState( MealyState state );
	void AddTransition( std::string stateName, std::string nextStateName, std::string inputSignal, std::string outputSignal );
	void AddTransition( MealyTransition transition );
	std::set<MealyState> GetStates();
	std::set<MealyTransition> GetTransitions();
	std::set<MealyTransition> GetTransitions( std::string currentStateName );
	std::set<MealyTransition> GetTransitions( MealyState currentState );
	MealyTransition GetTransition( std::string currentStateName, std::string inputSignal );
	bool ContainsState( MealyState state ) const;
	bool ContainsState( std::string stateName ) const;
	bool ContainsTransition( MealyTransition transitions ) const;
	bool ContainsTransition( std::string stateName, std::string nextStateName, std::string inputSignal, std::string outputSignal ) const;
	void Save( std::string fileName, SaveFormat format );
	std::set<std::string> GetInputSignals();
private:
	std::set<MealyState> m_states;
	std::set<MealyTransition> m_transitions;

	void SaveInDot( std::string fileName );
	void SaveInTxt( std::string fileName );
};

