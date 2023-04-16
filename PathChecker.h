#ifndef PATH_CHECKER_H
#define PATH_CHECKER_H

#include "ArgumentInterface.h"

class PathChecker : public ArgumentInterface
{
public:
	PathChecker(const std::wstring& argumentIdentifier, int argc, wchar_t* argv[]);
	~PathChecker() override = default;

private:

};

#endif