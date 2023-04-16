#ifndef OUTPUT_PARSER_H
#define OUTPUT_PARSER_H

#include "ArgumentInterface.h"
#include <fstream>

class OutputParser : public ArgumentInterface
{
public:
	OutputParser(const std::wstring& argumentIdentifier, int argc, wchar_t* argv[]);
	~OutputParser() override = default;

private:
	std::ofstream m_OutputFile;
};

#endif