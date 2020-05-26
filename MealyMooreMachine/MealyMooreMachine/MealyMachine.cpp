#include "MealyMachine.h"
#include <fstream>
#include <string>

MealyMachine::MealyMachine() {
}

MealyMachine::~MealyMachine() {
}

void MealyMachine::AddState( MealyState state ) {
	bool isStates = false;
	for ( size_t i = 0; i != m_states.size() || isStates; i++ )
	{
		if ( *m_states[i] == state )
		{
			isStates = true;
		}
	}
	if ( !isStates )
	{
		m_states.push_back( std::make_shared<MealyState>(state) );
	}
	else
	{
		throw "This condition is already present: " + state.name;
	}
}

void MealyMachine::AddTransition( MealyState state, MealyState nextState, std::string inputSignal, std::string outputSignal ) {
	std::shared_ptr<MealyState> statePtr = nullptr;
	std::shared_ptr<MealyState> nextStatePtr = nullptr;
	for ( size_t i = 0; i < m_states.size(); i++ )
	{
		if ( *m_states[i] == state )
		{
			statePtr = m_states[i];
		}
		if ( *m_states[i] == nextState )
		{
			nextStatePtr = m_states[i];
		}
	}
	if ( state == nextState && statePtr == nullptr && nextStatePtr == nullptr )
	{
		this->AddState( state );
		statePtr = m_states.back();
		nextStatePtr = m_states.back();
	}
	if ( statePtr == nullptr )
	{
		this->AddState( state );
		statePtr = m_states.back();
	}
	if ( nextStatePtr == nullptr )
	{
		this->AddState( nextState );
		nextStatePtr = m_states.back();
	}
	m_transitions.push_back( { statePtr, nextStatePtr, inputSignal, outputSignal } );
}

const std::vector<std::shared_ptr<MealyState>>& MealyMachine::GetStates() const {
	return m_states;
}

const std::vector<MealyTransition>& MealyMachine::GetTransitions() {
	return m_transitions;
}

bool MealyMachine::isInStates( MealyState inState ) const {
	for ( std::shared_ptr<MealyState> state : m_states )
	{
		if ( inState == *state )
		{
			return true;
		}
	}
	return false;
}

bool MealyMachine::isInTransitions( MealyTransition inTransition ) const {
	for ( MealyTransition transition : m_transitions )
	{
		if ( inTransition == transition )
		{
			return true;
		}
	}
	return false;
}

void MealyMachine::save( std::string fileName ) {
	std::ofstream outFile( fileName + ".dot" );
	outFile << "digraph G {\n";
	for ( std::shared_ptr<MealyState> state : m_states )
	{
		outFile << state->name << ";\n";
	}
	for ( MealyTransition transition : m_transitions )
	{
		outFile << transition.currentState->name
			<< " -> "
			<< transition.nextState->name
			<< " [label=\""
			<< transition.inputSignal << "," << transition.outputSignal
			<< "\"];\n";
	}
	outFile << "}\n";
}
