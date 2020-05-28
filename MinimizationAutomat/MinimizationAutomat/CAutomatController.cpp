#include "CAutomatController.h"
#include <sstream>
#include <vector>

CAutomatController::CAutomatController( std::ifstream& input ): m_input(input) {
	m_automatType = AutomatType::UNKNOWN;
}

void CAutomatController::Run() {
	size_t amountState;
	size_t amountInputSignal;
	std::string type;
	m_input >> amountInputSignal >> amountState >> type;
	SetAutomat( type );
	if ( m_automatType == AutomatType::MEALY )
	{
		CAutomatMealy mealy = ReadMealy( amountState, amountInputSignal );
		mealy.Save( SaveInputMealyFile, MEALY_SAVE_FORMAT );
		Minimization minimization = Minimization();
		mealy = minimization.MinimizeMealy( mealy );
		mealy.Save( SaveOutputMealyFile, MEALY_SAVE_FORMAT );
	}
	else if ( m_automatType == AutomatType::MOORE )
	{
		CAutomatMoore moore = ReadMoore( amountState, amountInputSignal );
		moore.Save( SaveInputMooreFile, MOORE_SAVE_FORMAT );
	}
}

void CAutomatController::SetAutomat( std::string type ) {
	if ( type == "mealy" )
	{
		m_automatType = AutomatType::MEALY;
	}
	else if ( type == "moore" )
	{
		m_automatType = AutomatType::MOORE;
	}
	else
	{
		m_automatType = AutomatType::UNKNOWN;
	}
}

CAutomatMealy CAutomatController::ReadMealy( const size_t amountState, const size_t amountInputSignal ) {
	CAutomatMealy mealy = CAutomatMealy();
	std::vector<std::string> states;
	for ( size_t i = 0; i < amountState; i++ )
	{
		std::string state;
		m_input >> state;
		states.push_back( state );
		mealy.AddState(state);
	}
	for ( size_t i = 0; i < amountInputSignal; i++ )
	{
		std::string inputSignal;
		m_input >> inputSignal;
		for ( size_t j = 0; j < amountState; j++ )
		{
			std::string stateWithOutputSignal;
			m_input >> stateWithOutputSignal;
			std::stringstream stream( stateWithOutputSignal );
			std::string state;
			std::string outputSignal;
			std::getline( stream, state, '|' );
			std::getline( stream, outputSignal );
			mealy.AddTransition( states[j], state, inputSignal, outputSignal );
		}
	}
	return mealy;
}

CAutomatMoore CAutomatController::ReadMoore( const size_t amountState, const size_t amountInputSignal ) {
	CAutomatMoore moore = CAutomatMoore();
	std::vector<MooreState> states;
	for ( size_t i = 0; i < amountState; i++ )
	{
		std::string stateWithOutputSignal;
		m_input >> stateWithOutputSignal;
		std::stringstream stream( stateWithOutputSignal );
		std::string state;
		std::string outputSignal;
		std::getline( stream, state, '|' );
		std::getline( stream, outputSignal );
		states.push_back( { state, outputSignal } );
		moore.AddState( state, outputSignal );
	}
	for ( size_t i = 0; i < amountInputSignal; i++ )
	{
		std::string inputSignal;
		m_input >> inputSignal;
		for ( size_t j = 0; j < amountState; j++ )
		{
			std::string state;
			m_input >> state;
			MooreState mooreState = moore.GetState( state );
			moore.AddTransition( states[j], mooreState, inputSignal );
		}
	}
	return moore;
}
