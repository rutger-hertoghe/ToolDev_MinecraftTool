#include "PathChecker.h"

#include <sstream>
#include <fstream>
#include <filesystem>

PathChecker::PathChecker(const std::wstring& argumentIdentifier, int argc, wchar_t* argv[])
{
	const auto path{ ParseArgument(argc, argv, argumentIdentifier) };
	std::wstringstream tempFileName{};
	tempFileName << path << "\\.temp";
	std::ofstream{ tempFileName.str()};


}
