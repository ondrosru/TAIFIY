#include "CAutomatMealy.h"

CAutomatMealy::CAutomatMealy() {
}

CAutomatMealy::~CAutomatMealy() {
}

void CAutomatMealy::AddState( std::string stateName ) {
	m_states.insert( { stateName } );
}

void CAutomatMealy::AddState( MealyState state ) {
	m_states.insert( state );
}

void CAutomatMealy::AddTransition( std::string stateName, std::string nextStateName, std::string inputSignal, std::string outputSignal ) {
	if ( m_states.find( { stateName } ) == m_states.end() )
	{
		AddState( stateName );
	}
	if ( m_states.find( { nextStateName } ) == m_states.end() )
	{
		AddState( nextStateName );
	}
	m_transitions.insert( { stateName, nextStateName, inputSignal, outputSignal } );
}

void CAutomatMealy::AddTransition( MealyTransition transition ) {
	if ( m_states.find( { transition.currentState } ) == m_states.end() )
	{
		AddState( transition.currentState );
	}
	if ( m_states.find( { transition.nextState } ) == m_states.end() )
	{
		AddState( transition.nextState );
	}
	m_transitions.insert( transition );
}

std::set<MealyState> CAutomatMealy::GetStates() {
	return m_states;
}

std::set<MealyTransition> CAutomatMealy::GetTransitions() {
	return m_transitions;
}

std::set<MealyTransition> CAutomatMealy::GetTransitions( std::string currentStateName ) {
	std::set<MealyTransition> foundTransitions;
	for ( auto transition : m_transitions )
	{
		if ( transition.currentState == currentStateName )
		{
			foundTransitions.insert( transition );
		}
	}
	return foundTransitions;
}

std::set<MealyTransition> CAutomatMealy::GetTransitions( MealyState currentState ) {
	
	return GetTransitions( currentState.name );
}

MealyTransition CAutomatMealy::GetTransition( std::string currentStateName, std::string inputSignal ) {
	for ( auto transition : m_transitions )
	{
		if ( transition.currentState == currentStateName && transition.inputSignal == inputSignal )
		{
			return transition;
		}
	}
	return MealyTransition();
}

bool CAutomatMealy::ContainsState( MealyState state ) const {
	return m_states.find( state ) != m_states.end();
}

bool CAutomatMealy::ContainsState( std::string stateName ) const {
	return m_states.find( {stateName} ) != m_states.end();
}

bool CAutomatMealy::ContainsTransition( MealyTransition transitions ) const {
	return m_transitions.find( transitions ) != m_transitions.end();
}

bool CAutomatMealy::ContainsTransition( std::string stateName, std::string nextStateName, std::string inputSignal, std::string outputSignal ) const {
	return m_transitions.find( {stateName, nextStateName, inputSignal, outputSignal} ) != m_transitions.end();
}

void CAutomatMealy::Save( std::string fileName, SaveFormat format ) {
	if ( format == SaveFormat::DOT )
	{
		SaveInDot( fileName );
	}
	else if ( format == SaveFormat::TXT )
	{
		SaveInTxt( fileName );
	}
}

std::set<std::string> CAutomatMealy::GetInputSignals() {
	std::set<std::string> inputSignals;
	for ( auto transition: m_transitions )
	{
		if ( inputSignals.find( transition.inputSignal ) == inputSignals.end() )
		{
			inputSignals.insert( transition.inputSignal );
		}
	}
	return inputSignals;
}

void CAutomatMealy::SaveInDot( std::string fileName ) {
	std::ofstream file( fileName + ".dot" );
	file << "digraph G {" << std::endl;
	for ( auto state : m_states )
	{
		file << state.name << ";" << std::endl;
	}
	for ( auto transition : m_transitions)
	{
		file << transition.currentState << " -> "
			<< transition.nextState
			<< " [label=\""
			<< transition.inputSignal << ", " << transition.outputSignal
			<< "\"];" << std::endl;
	}
	file << "}" << std::endl;
}

void CAutomatMealy::SaveInTxt( std::string fileName ) {
	std::ofstream file( fileName + ".txt" );
	std::set<std::string> inputSignals = GetInputSignals();
	file << m_states.size() << std::endl;
	file << inputSignals.size() << std::endl;
	for ( auto state: m_states )
	{
		file << state.name << " ";
	}
	file << std::endl;
	for ( auto inputSignal: inputSignals )
	{
		file << inputSignal << " ";
		for ( auto state : m_states )
		{
			MealyTransition foundTransition = GetTransition(state.name , inputSignal);
			file << foundTransition.nextState << "|" << foundTransition.outputSignal << " ";
		}
		file << std::endl;
	}
}
