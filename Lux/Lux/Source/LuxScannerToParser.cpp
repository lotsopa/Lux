#include "LuxPCH.h"
#include "LuxScannerToParser.h"


void ParseIdentifier(char* a_Text, const char* a_File)
{
	std::cout << "An Identifier: " << a_Text << std::endl;
}

void ParseOpenBrace(const char* a_File)
{
	std::cout << "Open Brace" << std::endl;
}

void ParseCloseBrace(const char* a_File)
{
	std::cout << "Close Brace" << std::endl;
}

void ParseSemiColon(const char* a_File)
{
	std::cout << "Semi Colon" << std::endl;
}

void ParseEquals(const char* a_File)
{
	std::cout << "Equals" << std::endl;
}

void ParseString(char* a_Text, const char* a_File)
{
	std::cout << "A String: " << a_Text << std::endl;
}
