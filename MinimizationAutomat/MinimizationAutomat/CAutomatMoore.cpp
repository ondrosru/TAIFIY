#include "CAutomatMoore.h"

CAutomatMoore::CAutomatMoore() {
}

CAutomatMoore::~CAutomatMoore() {
}

void CAutomatMoore::AddState( std::string stateName, std::string outputSignal ) {
	m_states.insert( { stateName, outputSignal } );
}

void CAutomatMoore::AddState( MooreState state ) {
	m_states.insert( state );
}

void CAutomatMoore::AddTransition( MooreState currentState, MooreState nextState, std::string inputSignal ) {
	if ( m_states.find( currentState ) == m_states.end() )
	{
		AddState( currentState );
	}
	if ( m_states.find( nextState ) == m_states.end() )
	{
		AddState( nextState );
	}
	m_transitions.insert( { currentState, nextState, inputSignal } );
}

void CAutomatMoore::AddTransition( MooreTransition transition ) {
	if ( m_states.find( transition.currentState ) == m_states.end() )
	{
		AddState( transition.currentState );
	}
	if ( m_states.find( transition.nextState ) == m_states.end() )
	{
		AddState( transition.nextState );
	}
	m_transitions.insert( transition );
}

std::set<MooreState> CAutomatMoore::GetStates() {
	return m_states;
}

MooreState CAutomatMoore::GetState( std::string stateName ) {
	for ( auto state: m_states )
	{
		if ( state.name == stateName )
		{
			return state;
		}
	}

	return MooreState();
}

std::set<MooreTransition> CAutomatMoore::GetTransitions() {
	return m_transitions;
}

std::set<MooreTransition> CAutomatMoore::GetTransitions( MooreState currentState ) {
	std::set<MooreTransition> transitions;
	for ( auto transition: m_transitions )
	{
		if ( transition.currentState == currentState )
		{
			transitions.insert( transition );
		}
	}
	return transitions;
}

MooreTransition CAutomatMoore::GetTransition( MooreState currentState, std::string inputSignal ) {
	for ( auto transition: m_transitions )
	{
		if ( transition.currentState == currentState && transition.inputSignal == inputSignal )
		{
			return transition;
		}
	}
	return MooreTransition();
}

bool CAutomatMoore::ContainsState( MooreState state ) {
	return m_states.find(state) != m_states.end();
}

bool CAutomatMoore::ContainsTransition( MooreTransition transition ) {
	return m_transitions.find(transition) != m_transitions.end();
}

std::set<std::string> CAutomatMoore::GetInputSignals() {
	std::set<std::string> inputSignals;
	for ( auto transition : m_transitions )
	{
		if ( inputSignals.find( transition.inputSignal ) == inputSignals.end() )
		{
			inputSignals.insert( transition.inputSignal );
		}
	}
	return inputSignals;
}

void CAutomatMoore::Save( std::string fileName, SaveFormat format ) {
	if ( format == SaveFormat::DOT )
	{
		SaveInDot( fileName );
	}
	else if ( format == SaveFormat::TXT )
	{
		SaveInTxt( fileName );
	}
}

void CAutomatMoore::SaveInDot( std::string fileName ) {
	std::ofstream file( fileName + ".dot" );
	file << "digraph G {" << std::endl;
	for ( auto state: m_states)
	{
		file << "\"" << state.name << ", " << state.outputSignal << "\";" << std::endl;
	}
	for ( auto transition: m_transitions)
	{
		file << "\"" << transition.currentState.name << ", " << transition.currentState.outputSignal << "\""
			<< " -> "
			<< "\"" << transition.nextState.name << ", " << transition.nextState.outputSignal << "\""
			<< " [label=\"" << transition.inputSignal << "\"];" << std::endl;
	}
	file << "}" << std::endl;
}

void CAutomatMoore::SaveInTxt( std::string fileName ) {
	std::ofstream file( fileName + ".txt" );
	std::set<std::string> inputSignals = GetInputSignals();
	file << m_states.size() << std::endl;
	file << inputSignals.size() << std::endl;
	for ( auto state: m_states )
	{
		file << state.name << "|" << state.outputSignal << " ";
	}
	file << std::endl;
	for ( auto inputSignal: inputSignals )
	{
		file << inputSignal << " ";
		for ( auto state: m_states )
		{
			MooreTransition transition = GetTransition( state, inputSignal );
			file << transition.nextState.name << " ";
		}
		file << std::endl;
	}
}
