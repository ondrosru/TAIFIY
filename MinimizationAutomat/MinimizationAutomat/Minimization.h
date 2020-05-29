#pragma once
#include "CAutomatMealy.h"
#include "CAutomatMoore.h"
#include "Header.h"
#include <vector>

using DoubelVectorString = std::vector<std::vector<std::string>>;

class Minimization
{
public:
	Minimization();
	~Minimization();
	CAutomatMoore MinimizeMoore( CAutomatMoore& moore );
	CAutomatMealy MinimizeMealy( CAutomatMealy& mealy );
private:
	DoubelVectorString GettingGroupStatesMealy( CAutomatMealy& mealy );
	DoubelVectorString GettingGroupStatesMoore( CAutomatMoore& moore );
	DoubelVectorString MinimizeMealyRecursion( CAutomatMealy& mealy, DoubelVectorString& groupStates );
	DoubelVectorString MinimizeMooreRecursion( CAutomatMoore& moore, DoubelVectorString& groupStates );
	CAutomatMealy CreateMealy( CAutomatMealy& mealy, DoubelVectorString& groupStates );
	CAutomatMoore CreateMoore( CAutomatMoore& moore, DoubelVectorString& groupStates );
};

