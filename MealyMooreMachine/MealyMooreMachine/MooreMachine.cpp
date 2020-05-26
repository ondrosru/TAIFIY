#include "MooreMachine.h"
#include <fstream>
#include <string>


MooreMachine::MooreMachine() {
}

MooreMachine::~MooreMachine() {
}

void MooreMachine::AddState( MooreState state ) {
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
		m_states.push_back( std::make_shared<MooreState>(state) );
	}
	else
	{
		throw "This condition is already present: " + state.name;
	}
}

void MooreMachine::AddTransition( MooreState state, MooreState nextState, std::string inputSignal ) {
	std::shared_ptr<MooreState> statePtr = nullptr;
	std::shared_ptr<MooreState> nextStatePtr = nullptr;
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
	m_transitions.push_back( { statePtr, nextStatePtr, inputSignal } );

}

const std::vector<std::shared_ptr<MooreState>>& MooreMachine::GetStates() const {
	return m_states;
}

const std::vector<MooreTransition>& MooreMachine::GetTransitions() const {
	return m_transitions;
}

bool MooreMachine::isInStates( MooreState inState ) const {
	for ( std::shared_ptr<MooreState> state : m_states )
	{
		if ( *state == inState )
		{
			return true;
		}
	}
	return false;
}

bool MooreMachine::isInTransitions( MooreTransition inTransition ) const {
	for ( MooreTransition transition : m_transitions )
	{
		if ( transition == inTransition )
		{
			return true;
		}
	}
	return false;
}

void MooreMachine::save( std::string fileName ) {
	std::ofstream outFile( fileName + ".dot" );
	outFile << "digraph G {\n";
	for ( std::shared_ptr<MooreState> state : m_states)
	{
		outFile << state->name << "_" << state->outputSignal << ";\n";
	}

	for ( MooreTransition transition : m_transitions )
	{
		outFile << transition.currentState->name << "_" << transition.currentState->outputSignal
			<< " -> "
			<< transition.nextState->name << "_" << transition.nextState->outputSignal
			<< " [label=\""
			<< transition.inputSignal
			<< "\"];\n";
	}
	outFile << "}\n";
}
