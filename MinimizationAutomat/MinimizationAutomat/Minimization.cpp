#include "Minimization.h"
#include <map>


Minimization::Minimization() {
}

Minimization::~Minimization() {
}

CAutomatMoore Minimization::MinimizeMoore( CAutomatMoore& moore ) {
	DoubelVectorString groupStates = GettingGroupStatesMoore( moore );
	groupStates = MinimizeMooreRecursion( moore, groupStates );
	return CreateMoore( moore, groupStates );
}

CAutomatMealy Minimization::MinimizeMealy( CAutomatMealy& mealy ) {
	DoubelVectorString groupStates = GettingGroupStatesMealy( mealy );
	groupStates = MinimizeMealyRecursion( mealy, groupStates );
	return CreateMealy(mealy, groupStates);
}

DoubelVectorString Minimization::GettingGroupStatesMealy( CAutomatMealy& mealy ) {
	std::vector<std::map<std::string, std::string>> equivalentOutputSignals;
	DoubelVectorString equivalentStates;
	std::set<std::string> inputSignals = mealy.GetInputSignals();
	std::set<MealyState> states = mealy.GetStates();
	for ( MealyState state : states )
	{
		int index = 0;
		int foundIndex = -1;
		for ( std::map<std::string, std::string> outputSignals : equivalentOutputSignals )
		{
			bool flag = true;
			for ( std::string inputSignal : inputSignals )
			{
				if ( outputSignals[inputSignal] != mealy.GetTransition( state.name, inputSignal ).outputSignal )
				{
					flag = false;
					break;
				}
			}

			if ( flag )
			{
				foundIndex = index;
				break;
			}
			index++;
		}

		if ( foundIndex != -1 )
		{
			equivalentStates[foundIndex].push_back( state.name );
		}
		else
		{
			std::map<std::string, std::string> newOutputSignals;
			for ( std::string inputSignal : inputSignals )
			{
				newOutputSignals[inputSignal] = mealy.GetTransition( state.name, inputSignal ).outputSignal;
			}
			equivalentOutputSignals.push_back( newOutputSignals );
			std::vector<std::string> newStatesGroup;
			newStatesGroup.push_back( state.name );
			equivalentStates.push_back( newStatesGroup );
		}
	}
	return equivalentStates;
}

DoubelVectorString Minimization::GettingGroupStatesMoore( CAutomatMoore& moore ) {
	std::vector<std::string> equivalentOutputSignals;
	DoubelVectorString equivalentStates;
	std::set<MooreState> states = moore.GetStates();
	for ( MooreState state: states )
	{
		size_t foundIndex = -1;
		size_t index = 0;
		for ( std::string outputSignal : equivalentOutputSignals )
		{
			if ( outputSignal == state.outputSignal )
			{
				foundIndex = index;
				break;
			}
			index++;
		}
		if ( foundIndex != -1 )
		{
			equivalentStates[foundIndex].push_back( state.name );
		}
		else
		{
			std::vector<std::string> newStates;
			newStates.push_back( state.name );
			equivalentStates.push_back( newStates );
			equivalentOutputSignals.push_back( state.outputSignal );
		}
	}
	return equivalentStates;
}

DoubelVectorString Minimization::MinimizeMealyRecursion( CAutomatMealy& mealy, DoubelVectorString& groupStates ) {
	DoubelVectorString newGroupStates;
	std::set<std::string> inputSignals = mealy.GetInputSignals();
	for ( std::vector<std::string> states: groupStates )
	{
		DoubelVectorString newLocalGroupStates;
		std::vector<std::map<std::string, int>> equivalentNextStates;
		for ( size_t i = 0; i < states.size(); i++ )
		{
			std::map<std::string, int> convertedNextStates;
			for ( std::string inputSignal : inputSignals )
			{
				MealyTransition transition = mealy.GetTransition( states[i], inputSignal );
				size_t foundIndex;
				for ( foundIndex = 0; foundIndex < groupStates.size(); foundIndex++ )
				{
					bool foundState = false;
					for ( std::string state : groupStates[foundIndex] )
					{
						if ( state == transition.nextState )
						{
							foundState = true;
							break;
						}
					}
					if ( foundState )
					{
						break;
					}
				}
				convertedNextStates[inputSignal] = foundIndex;
			}
			int foundIndex = -1;
			int index = 0;
			for ( std::map<std::string, int> nextStates : equivalentNextStates )
			{
				bool flag = true;
				for ( std::string inputSignal : inputSignals )
				{
					if ( nextStates[inputSignal] != convertedNextStates[inputSignal] )
					{
						flag = false;
						break;
					}
				}
				if ( flag )
				{
					foundIndex = index;
					break;
				}
				index++;
			}

			if ( foundIndex != -1 )
			{
				newLocalGroupStates[index].push_back(states[i]);
			}
			else
			{
				std::map<std::string, int> newNextStates;
				for ( std::string inputSignal : inputSignals )
				{
					newNextStates[inputSignal] = convertedNextStates[inputSignal];
				}
				equivalentNextStates.push_back( newNextStates );
				std::vector<std::string> newStatesGroup;
				newStatesGroup.push_back( states[i] );
				newLocalGroupStates.push_back( newStatesGroup );
			}
		}
		for ( size_t i = 0; i < newLocalGroupStates.size(); i++ )
		{
			newGroupStates.push_back( newLocalGroupStates[i] );
		}
	}

	if ( newGroupStates.size() == groupStates.size() )
	{
		return newGroupStates;
	}
	return MinimizeMealyRecursion(mealy, newGroupStates);
}

DoubelVectorString Minimization::MinimizeMooreRecursion( CAutomatMoore& moore, DoubelVectorString& groupStates ) {
	DoubelVectorString newGroupStates;
	std::set<std::string> inputSignals = moore.GetInputSignals();
	for ( std::vector<std::string> states : groupStates )
	{
		DoubelVectorString newLocalGroupStates;
		std::vector<std::map<std::string, int>> equivalentNextStates;
		for ( size_t i = 0; i < states.size(); i++ )
		{
			std::map<std::string, int> convertedNextStates;
			for ( std::string inputSignal : inputSignals )
			{
				MooreTransition transition = moore.GetTransition( moore.GetState( states[i] ), inputSignal );
				size_t foundIndex;
				for ( foundIndex = 0; foundIndex < groupStates.size(); foundIndex++ )
				{
					bool foundState = false;
					for ( std::string state : groupStates[foundIndex] )
					{
						if ( state == transition.nextState.name )
						{
							foundState = true;
							break;
						}
					}
					if ( foundState )
					{
						break;
					}
				}
				convertedNextStates[inputSignal] = foundIndex;
			}
			int foundIndex = -1;
			int index = 0;
			for ( std::map<std::string, int> nextStates : equivalentNextStates )
			{
				bool flag = true;
				for ( std::string inputSignal : inputSignals )
				{
					if ( nextStates[inputSignal] != convertedNextStates[inputSignal] )
					{
						flag = false;
						break;
					}
				}
				if ( flag )
				{
					foundIndex = index;
					break;
				}
				index++;
			}

			if ( foundIndex != -1 )
			{
				newLocalGroupStates[index].push_back( states[i] );
			}
			else
			{
				std::map<std::string, int> newNextStates;
				for ( std::string inputSignal : inputSignals )
				{
					newNextStates[inputSignal] = convertedNextStates[inputSignal];
				}
				equivalentNextStates.push_back( newNextStates );
				std::vector<std::string> newStatesGroup;
				newStatesGroup.push_back( states[i] );
				newLocalGroupStates.push_back( newStatesGroup );
			}
		}
		for ( size_t i = 0; i < newLocalGroupStates.size(); i++ )
		{
			newGroupStates.push_back( newLocalGroupStates[i] );
		}
	}
	if ( newGroupStates.size() == groupStates.size() )
	{
		return newGroupStates;
	}
	return MinimizeMooreRecursion( moore, newGroupStates );
}

CAutomatMealy Minimization::CreateMealy( CAutomatMealy& mealy, DoubelVectorString& groupStates ) {
	CAutomatMealy newMealy = CAutomatMealy();
	std::set<std::string> inputSignals = mealy.GetInputSignals();
	for ( size_t i = 0; i < groupStates.size(); i++ )
	{
		std::string newName = SYMBOL_Z + "_" + std::to_string( i );
		for ( auto inputSignal: inputSignals )
		{
			MealyTransition transition = mealy.GetTransition( groupStates[i][0], inputSignal );
			std::string nextStateName = "";
			for ( size_t j = 0; j < groupStates.size(); j++ )
			{
				for ( auto state: groupStates[j] )
				{
					if ( state == transition.nextState )
					{
						nextStateName = SYMBOL_Z + "_" + std::to_string( j );
						break;
					}
				}
				if ( nextStateName != "" )
				{
					break;
				}
			}
			newMealy.AddTransition( newName, nextStateName, transition.inputSignal, transition.outputSignal );
		}
	}
	return newMealy;
}

CAutomatMoore Minimization::CreateMoore( CAutomatMoore& moore, DoubelVectorString& groupStates ) {
	CAutomatMoore newMoore = CAutomatMoore();
	std::set<std::string> inputSignals = moore.GetInputSignals();
	for ( size_t i = 0; i < groupStates.size(); i++ )
	{
		std::string newName = SYMBOL_Z + "_" + std::to_string( i );
		for ( std::string inputSignal : inputSignals )
		{
			MooreTransition transition = moore.GetTransition( moore.GetState( groupStates[i][0] ), inputSignal );
			std::string nextStateName = "";
			for ( size_t j = 0; j < groupStates.size(); j++ )
			{
				for ( auto state : groupStates[j] )
				{
					if ( state == transition.nextState.name )
					{
						nextStateName = SYMBOL_Z + "_" + std::to_string( j );
						break;
					}
				}
				if ( nextStateName != "" )
				{
					break;
				}
			}
			newMoore.AddTransition( { newName, transition.currentState.outputSignal }, {nextStateName, transition.nextState.outputSignal}, inputSignal );
		}
	}
	return newMoore;
}
