#include "LuxPCH.h"
#include "LuxScannerToParser.h"
#include "LuxKey.h"
#include "LuxFileHandler.h"
#include "LuxShaderFileParser.h"

void ParseIdentifier(char* a_Text, const char* a_File)
{
	//std::cout << "An Identifier: " << a_Text << std::endl;
	Lux::Core::FileHandler::GetInstance().GetShaderParser(Lux::Core::Key(a_File)).ProcessIdentifier(Lux::Core::Key(a_Text));
}

void ParseOpenBrace(const char* a_File)
{
	//std::cout << "Open Brace" << std::endl;
	Lux::Core::FileHandler::GetInstance().GetShaderParser(Lux::Core::Key(a_File)).ProcessOpenBrace();
}

void ParseCloseBrace(const char* a_File)
{
	//std::cout << "Close Brace" << std::endl;
	Lux::Core::FileHandler::GetInstance().GetShaderParser(Lux::Core::Key(a_File)).ProcessCloseBrace();
}

void ParseSemiColon(const char* a_File)
{
	//std::cout << "Semi Colon" << std::endl;
	Lux::Core::FileHandler::GetInstance().GetShaderParser(Lux::Core::Key(a_File)).ProcessSemiColon();
}

void ParseEquals(const char* a_File)
{
	//std::cout << "Equals" << std::endl;
	Lux::Core::FileHandler::GetInstance().GetShaderParser(Lux::Core::Key(a_File)).ProcessEquals();
}

void ParseString(char* a_Text, const char* a_File)
{
	// Remove the extra double quotes
	std::string str(a_Text);
	str = str.substr(1, str.size() - 2);
	//std::cout << "A String: " << a_Text << std::endl;
	Lux::Core::FileHandler::GetInstance().GetShaderParser(Lux::Core::Key(a_File)).ProcessString(Lux::Core::Key(str));
}
