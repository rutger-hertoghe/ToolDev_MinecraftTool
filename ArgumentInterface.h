#ifndef ARGUMENT_INTERFACE_H
#define ARGUMENT_INTERFACE_H

#include <string>

class ArgumentInterface
{
public:
	ArgumentInterface();
	virtual ~ArgumentInterface() = 0;

	std::wstring ParseArgument(int argc, wchar_t* argv[], const std::wstring& identifier);

	[[nodiscard]] bool IsValid() const;

protected:
	bool m_IsValid;
};

#endif