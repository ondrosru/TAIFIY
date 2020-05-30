#include "CAutomat.h"
#include <sstream>

CAutomat::CAutomat() {
}

CAutomat::CAutomat( std::ifstream& stream ) {
	m_states = std::vector<State>();
	m_transitions = std::vector<Transition>();
	size_t amountState;
	size_t amountInputSignal;
	stream >> amountInputSignal >> amountState;
	std::vector<State> states;
	for ( size_t i = 0; i < amountState; i++ )
	{
		std::string stateName;
		stream >> stateName;
		states.push_back( { stateName } );
		m_states.push_back( { stateName } );
	}
	for ( size_t i = 0; i < amountInputSignal; i++ )
	{
		std::string inputSignal;
		stream >> inputSignal;
		for ( size_t j = 0; j < amountState; j++ )
		{
			std::string nextStates;
			stream >> nextStates;
			if ( nextStates != "-" )
			{
				std::stringstream strStream( nextStates );
				while ( strStream.rdbuf()->in_avail() != 0 )
				{
					std::string nextStateName;
					std::getline( strStream, nextStateName, '|' );
					m_transitions.push_back( { states[j], {nextStateName}, inputSignal } );
				}
			}
		}
	}
}

CAutomat::~CAutomat() {
}

void CAutomat::AddState( State& state ) {
	m_states.push_back( state );
}

void CAutomat::AddTransition( Transition& transition ) {
	m_transitions.push_back( transition );
}

std::vector<Transition> CAutomat::GetTransitions( State& currentState ) {
	std::vector<Transition> transitions;
	for ( Transition transition : m_transitions )
	{
		if ( transition.currentState == currentState )
		{
			transitions.push_back( transition );
		}
	}
	return transitions;
}

std::vector<std::string> CAutomat::GetInputSignals() {
	std::vector<std::string> inputSignals;
	for ( Transition transition: m_transitions )
	{
		if ( std::find( inputSignals.begin(), inputSignals.end(), transition.inputSignal ) == inputSignals.end() )
		{
			inputSignals.push_back( transition.inputSignal );
		}
	}
	return inputSignals;
}

std::vector<Transition> CAutomat::GetTransitions( State& currentState, std::string inputSignal ) {
	std::vector<Transition> transitions;
	for ( size_t i = 0; i < m_transitions.size(); i++ )
	{
		if ( m_transitions[i].currentState == currentState && m_transitions[i].inputSignal == inputSignal )
		{
			transitions.push_back( m_transitions[i] );
		}
	}
	return std::vector<Transition>();
}

void CAutomat::SaveInDot( std::string fileName ) {
	std::ofstream output( fileName + ".dot" );
	output << "digraph G {" << std::endl;
	for ( State state : m_states )
	{
		output << "\"" << state.name << "\";" << std::endl;
	}
	for ( Transition transition : m_transitions )
	{
		output << "\"" << transition.currentState.name << "\""
			<< " -> "
			<< "\"" << transition.nextState.name << "\""
			<< " [label=\"" << transition.inputSignal << "\"];" << std::endl;
	}
	output << "}" << std::endl;
}

void CAutomat::Determine() {
	std::vector<std::string> inputSignals = GetInputSignals();
	std::vector<AuxiliaryTransition> newTransitions;
	std::vector<std::vector<State>> newStates;
	std::vector<State> startState;
	startState.push_back( m_states[0] );
	newStates.push_back( startState );
	for ( size_t i = 0; i < newStates.size(); i++ )
	{
		for ( size_t j = 0; j < inputSignals.size(); j++ )
		{
			std::vector<Transition> foundTransitions = GetTransitions( newStates[i], inputSignals[j] );
			if (!foundTransitions.empty())
			{
				std::vector<State> transitionsNextStates;
				for ( Transition transition : foundTransitions )
				{
					bool contains = false;
					for ( State nextState : transitionsNextStates )
					{
						if ( nextState == transition.nextState )
						{
							contains = true;
							break;
						}
					}
					if ( !contains )
					{
						transitionsNextStates.push_back( transition.nextState );
					}
				}
				ShakerSortStates( transitionsNextStates );
				bool flag = false;
				for ( std::vector<State> states : newStates )
				{
					flag = false;
					if ( transitionsNextStates.size() == states.size() )
					{
						flag = true;
						for ( size_t n = 0; n < states.size(); n++ )
						{
							if ( !(transitionsNextStates[n] == states[n]) )
							{
								flag = false;
								break;
							}
						}
					}

					if ( flag )
					{
						break;
					}
				}

				if ( !flag )
				{
					newStates.push_back( transitionsNextStates );
				}

				newTransitions.push_back( { newStates[i], transitionsNextStates, inputSignals[j] } );
			}
		}
	}
	Update( newTransitions, newStates);
}

std::vector<Transition> CAutomat::GetTransitions( std::vector<State>& states, std::string inputSignal ) {
	std::vector<Transition> transitions;
	for ( size_t i = 0; i < m_transitions.size(); i++ )
	{
		for ( size_t j = 0; j < states.size(); j++ )
		{
			if ( states[j].name == m_transitions[i].currentState.name && m_transitions[i].inputSignal == inputSignal )
			{
				transitions.push_back( m_transitions[i] );
			}
		}
	}
	return transitions;
}

void CAutomat::ShakerSortStates( std::vector<State>& states ) {
	if ( states.empty() )
	{
		return;
	}
	int left = 0;
	int right = states.size() - 1;
	while ( left <= right )
	{
		for ( int i = right; i > left; --i )
		{
			if ( states[i - 1].name > states[i].name )
			{
				std::swap( states[i - 1], states[i] );
			}
		}
		++left;
		for ( int i = left; i < right; ++i )
		{
			if ( states[i].name > states[i + 1].name )
			{
				std::swap( states[i], states[i + 1] );
			}
		}
		--right;
	}
}

void CAutomat::Update( std::vector<AuxiliaryTransition>& newTransitions, std::vector<std::vector<State>>& newStates ) {
	m_states.clear();
	m_transitions.clear();
	for ( size_t i = 0; i < newTransitions.size(); i++ )
	{
		std::string currentStateName = "";
		for ( State state : newTransitions[i].currentStates )
		{
			currentStateName += state.name;
		}
		std::string nextStateName = "";
		for ( State state : newTransitions[i].nextStates )
		{
			nextStateName += state.name;
		}
		State currentState{ currentStateName };
		if ( std::find( m_states.begin(), m_states.end(), currentState ) == m_states.end() )
		{
			m_states.push_back( currentState );
		}
		State nextState{ nextStateName };
		if ( std::find( m_states.begin(), m_states.end(), nextState ) == m_states.end() )
		{
			m_states.push_back( nextState );
		}
		m_transitions.push_back( { currentState, nextState, newTransitions[i].inputSignal } );
	}
}
