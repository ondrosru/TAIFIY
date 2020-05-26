#pragma once
#include <string>
#include <iostream>

struct MealyState
{
	std::string name;

	bool operator==( const MealyState& state ) {
		return state.name == this->name;
	}
};

struct MooreState
{
	std::string name;
	std::string outputSignal;

	bool operator==( const MooreState& state ) {
		return state.name == this->name && state.outputSignal == this->outputSignal;
	}
};


struct MealyTransition
{
	std::shared_ptr<MealyState> currentState;
	std::shared_ptr<MealyState> nextState;
	std::string inputSignal;
	std::string outputSignal;

	bool operator==( const MealyTransition& transition ) {
		return transition.currentState == this->currentState &&
			transition.nextState == this->nextState &&
			transition.inputSignal == this->inputSignal &&
			transition.outputSignal == this->outputSignal;
	}
};

struct MooreTransition
{
	std::shared_ptr<MooreState> currentState;
	std::shared_ptr<MooreState> nextState;
	std::string inputSignal;

	bool operator==( const MooreTransition& transition ) {
		return transition.currentState == this->currentState &&
			transition.nextState == this->nextState &&
			transition.inputSignal == this->inputSignal;
	}
};