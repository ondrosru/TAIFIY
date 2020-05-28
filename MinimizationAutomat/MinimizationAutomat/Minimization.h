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
	DoubelVectorString MinimizeMealyRecursion( CAutomatMealy& mealy, DoubelVectorString& groupStates );
	CAutomatMealy CreateMealy( CAutomatMealy& mealy, DoubelVectorString& groupStates );
};

