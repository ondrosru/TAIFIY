#pragma once
#include "CAutomatMealy.h"
#include "CAutomatMoore.h"
#include "Minimization.h"
#include "Header.h"

class CAutomatController
{
public:
	CAutomatController(std::ifstream& input);
	void Run();
private:
	std::ifstream& m_input;
	AutomatType m_automatType;

	void SetAutomat( std::string type );
	CAutomatMealy ReadMealy( const size_t amountState, const size_t amountInputSignal );
	CAutomatMoore ReadMoore( const size_t amountState, const size_t amountInputSignal );
};
