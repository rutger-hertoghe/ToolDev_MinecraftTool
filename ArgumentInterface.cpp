#include "ArgumentInterface.h"

#include <iostream>

ArgumentInterface::ArgumentInterface()
	: m_IsValid{false}
{
}

std::wstring ArgumentInterface::ParseArgument(int argc, wchar_t* argv[], const std::wstring& identifier)
{
    for (int i{ 0 }; i < argc; ++i)
    {
        const int argument{ i + i };
        if (static_cast<std::wstring>(argv[i]) == identifier && argument < argc)
        {
            return static_cast<std::wstring>(argv[argument]);
        }
    }
    m_IsValid = true;
    return std::wstring{};
}

bool ArgumentInterface::IsValid() const
{
    return m_IsValid;
}
