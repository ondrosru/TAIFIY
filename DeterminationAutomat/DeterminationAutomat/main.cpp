#include <iostream>
#include "Header.h"
#include "CAutomat.h"

int main(int argc, char* argv[]) {
	if ( argc != 2 )
	{
		std::cerr << ERROR_NOT_ENOUGH_ARGUMENTS;
		return 1;
	}

	std::ifstream fileInput( argv[1] );

	if ( !fileInput.is_open() )
	{
		std::cerr << ERROR_FILE_NOT_FOUND << " File: " << argv[1] << std::endl;
		return 1;
	}
	CAutomat automat = CAutomat( fileInput );
	automat.SaveInDot( "input" );
	automat.Determine();
	automat.SaveInDot( "output" );
	system("pause");
}