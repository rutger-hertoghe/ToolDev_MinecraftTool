#include "OutputParser.h"

OutputParser::OutputParser(const std::wstring& argumentIdentifier, int argc, wchar_t* argv[])
{
	m_OutputFile = std::ofstream{ ParseArgument(argc, argv, argumentIdentifier) };

}
