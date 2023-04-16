#ifndef INPUT_PARSER_H
#define INPUT_PARSER_H

#include "ArgumentInterface.h"
#include <fstream>

class InputParser final : public ArgumentInterface
{
public:
	InputParser(const std::wstring& argumentIdentifier, int argc, wchar_t* argv[]);
	~InputParser() override = default;

	std::ifstream& GetInputFile();

private:
	std::ifstream m_InputFile;
};

#endif