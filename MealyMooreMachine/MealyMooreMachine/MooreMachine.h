#pragma once
#include "Types.h"
#include <vector>

class MooreMachine
{
public:
	MooreMachine();
	~MooreMachine();
	void AddState( MooreState state );
	void AddTransition( MooreState state, MooreState nextState, std::string inputSignal );
	const std::vector<std::shared_ptr<MooreState>>& GetStates() const;
	const std::vector<MooreTransition>& GetTransitions() const;
	bool isInStates( MooreState inState ) const;
	bool isInTransitions( MooreTransition inTransition ) const;
	void save( std::string fileName );
private:
	std::vector<std::shared_ptr<MooreState>> m_states;
	std::vector<MooreTransition> m_transitions;
};

