
/*
This is the header which defines all the parse functions used by Flex.
By nature these functions are C functions, but act as wrappers to call C++ code.
These functions are the middle point between C and C++.
All the functions are implemented inside the LuxScannerToParser.cpp
*/
#ifdef __cplusplus
extern "C" 
{
#endif
	void ParseIdentifier(char* a_Text, const char* a_File);
	void ParseOpenBrace(const char* a_File);
	void ParseCloseBrace(const char* a_File);
	void ParseSemiColon(const char* a_File);
	void ParseEquals(const char* a_File);
	void ParseString(char* a_Text, const char* a_File);
#ifdef __cplusplus
}
#endif
