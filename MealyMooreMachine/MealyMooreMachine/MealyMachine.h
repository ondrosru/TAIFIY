#pragma once
#include "Types.h"
#include <vector>


class MealyMachine
{
public:
	MealyMachine();
	~MealyMachine();
	void AddState( MealyState state );
	void AddTransition( MealyState state, MealyState nextState, std::string inputSignal, std::string outputSignal );
	const std::vector<std::shared_ptr<MealyState>>& GetStates() const;
	const std::vector<MealyTransition>& GetTransitions();
	bool isInStates( MealyState inState ) const;
	bool isInTransitions( MealyTransition inTransition ) const;
	void save( std::string fileName);
private:
	std::vector<std::shared_ptr<MealyState>> m_states;
	std::vector<MealyTransition> m_transitions;
};
