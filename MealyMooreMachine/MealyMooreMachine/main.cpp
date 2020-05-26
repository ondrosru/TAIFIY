#include <iostream>
#include "MooreMachine.h"
#include "MealyMachine.h"
#include <string>
#include <fstream>
#include <cstring>
#include <sstream>
#include <map>

MooreMachine convert( MealyMachine& mealy );
MealyMachine convert( MooreMachine& moore );
MooreMachine mooreRead( std::ifstream& stream, int amountState, int amountInputSignal);
MealyMachine mealyRead( std::ifstream& stream, int amountState, int amountInputSignal);

int main(int argc, char *argv[]) {
	if ( argc != 2 )
	{
		std::cout << "MealyMooreMachine.exe [filename]" << std::endl;
		return 1;
	}
	std::ifstream file( argv[1] );
	if ( !file.is_open() )
	{
		std::cout << "Сould not open file: " << argv[1] << std::endl;
		return 1;
	}

	int numOfInputCharsAlphabet;
	int numOfOutCharsAlphabet;
	int numOfState;
	std::string typeOfMachine;
	
	file >> numOfInputCharsAlphabet >> numOfOutCharsAlphabet >> numOfState >> typeOfMachine;

	if ( typeOfMachine == "moore" )
	{
		MooreMachine moore = mooreRead( file, numOfState, numOfInputCharsAlphabet );
		MealyMachine mealy = convert( moore );
		mealy.save( "mealy_out" );
		moore.save( "moore_in" );
	}
	else if ( typeOfMachine == "mealy" )
	{
		MealyMachine mealy = mealyRead( file, numOfState, numOfInputCharsAlphabet );
		MooreMachine moore = convert( mealy );
		moore.save( "moore_out" );
		mealy.save( "mealy_in" );
	}
	else
	{
		std::cout << "unknown machine" << std::endl;
	}
	return 0;
}



MooreMachine convert( MealyMachine& mealy ) {
	std::vector<MealyTransition> mealyTransitions = mealy.GetTransitions();
	MooreMachine moore = MooreMachine();
	for ( std::vector<MealyTransition>::const_iterator it = mealyTransitions.begin(); it != mealyTransitions.end(); it++ )
	{
		if ( !moore.isInStates( { it->nextState->name, it->outputSignal } ) )
		{
			moore.AddState( { it->nextState->name, it->outputSignal } );
		}
	}
	std::vector<std::shared_ptr<MooreState>> mooreStates = moore.GetStates();
	for ( std::shared_ptr<MooreState> state : mooreStates )
	{
		for ( MealyTransition transition : mealyTransitions )
		{
			if ( state->name == transition.currentState->name )
			{
				moore.AddTransition( *state, { transition.nextState->name, transition.outputSignal }, transition.inputSignal );
			}
		}
	}
	return moore;
}

MealyMachine convert( MooreMachine& moore ) {
	MealyMachine mealy = MealyMachine();
	std::string newName = "Y";
	std::vector<MooreTransition> moooreTransitions = moore.GetTransitions();
	std::vector<std::shared_ptr<MooreState>> mooreStates = moore.GetStates();
	for ( size_t i = 0; i < mooreStates.size(); i++ )
	{
		mealy.AddState( { newName + std::to_string( i ) } );
	}
	for ( size_t i = 0; i < mooreStates.size(); i++ )
	{
		for ( MooreTransition transition : moooreTransitions )
		{
			if ( *mooreStates[i] == *transition.nextState )
			{
				size_t j = 0;
				while ( j < mooreStates.size() && !(*transition.currentState == *mooreStates[j]) )
				{
					j++;
				}
				mealy.AddTransition( { newName + std::to_string( j ) }, { newName + std::to_string( i ) }, transition.inputSignal, mooreStates[i]->outputSignal );
			}
		}
	}
	return mealy;
}

MooreMachine mooreRead( std::ifstream& stream, int amountState, int amountInputSignal ) {
	MooreMachine moore = MooreMachine();
	std::vector<MooreState> states;
	for ( int i = 0; i < amountState; i++ )
	{
		std::string nextStateWithSignal;
		stream >> nextStateWithSignal;
		std::string state;
		std::string outputSignal;
		std::stringstream strStream( nextStateWithSignal );
		std::getline( strStream, state, '|' );
		std::getline( strStream, outputSignal );
		states.push_back( { state, outputSignal } );
	}
	for ( int i = 0; i < amountInputSignal; i++ )
	{
		std::string inputSignal;
		stream >> inputSignal;
		for ( int j = 0; j < amountState; j++ )
		{
			std::string state;
			stream >> state;
			MooreState nextState;
			for ( MooreState tempState : states )
			{
				if ( tempState.name == state )
				{
					nextState = tempState;
					break;
				}
			}
			moore.AddTransition( states[j], nextState, inputSignal );
		}
	}
	return moore;
}

MealyMachine mealyRead( std::ifstream& stream, int amountState, int amountInputSignal ) {
	MealyMachine mealy = MealyMachine();
	std::vector<std::string> states;
	for ( int i = 0; i < amountState; i++ )
	{
		std::string state;
		stream >> state;
		states.push_back( state );
	}
	for ( int i = 0; i < amountInputSignal; i++ )
	{
		std::string inputSignal;
		stream >> inputSignal;
		for ( int j = 0; j < amountState; j++ )
		{
			std::string nextStateWithSignal;
			stream >> nextStateWithSignal;
			std::string state;
			std::string outputSignal;
			std::stringstream strStream( nextStateWithSignal );
			std::getline( strStream, state, '|' );
			std::getline( strStream, outputSignal );
			mealy.AddTransition( { states[j] }, {state}, inputSignal, outputSignal );
		}
	}
	return mealy;
}
