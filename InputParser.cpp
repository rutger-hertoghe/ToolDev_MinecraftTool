#include "InputParser.h"

#include <iostream>

InputParser::InputParser(const std::wstring& argumentIdentifier, int argc, wchar_t* argv[])
{
	const auto filename{ ParseArgument(argc, argv, argumentIdentifier) };
	m_InputFile = std::ifstream{ filename };

	if(m_InputFile)
	{
		std::wcout << "Input file is " << filename << "\n";
		m_IsValid = true;
	}
	std::wcout << "Specified input file was invalid!\n";
	m_IsValid = false;
}

std::ifstream& InputParser::GetInputFile()
{
	return m_InputFile;
}
