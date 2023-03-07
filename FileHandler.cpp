#include "FileHandler.h"

class InputFileNotFound{};

FileHandler::FileHandler(int argc, wchar_t* argv[], const std::string& defaultOutputFile )
	: m_DefaultOutputFile{defaultOutputFile}
{
    CheckForInputFile(argc, argv);
    CheckForOutputFile(argc, argv);
}

std::ifstream& FileHandler::GetInputFile()
{
    return m_InputFile;
}

std::ofstream& FileHandler::GetOutputFile()
{
    return m_OutputFile;
}

void FileHandler::CheckForInputFile(int argc, wchar_t* argv[])
{
    for (int i{ 0 }; i < argc; ++i)
    {
        if (static_cast<std::wstring>(argv[i]) == L"-i" && i + 1 < argc)
        {
            m_InputFile = std::ifstream{ argv[i + 1] };
            return;
        }
    }
    throw InputFileNotFound{};
}

void FileHandler::CheckForOutputFile(int argc, wchar_t* argv[])
{
    for (int i{ 0 }; i < argc; ++i)
    {
        if (static_cast<std::wstring>(argv[i]) == L"-o" && i + 1 < argc)
        {
            m_OutputFile = std::ofstream{ argv[i + 1] };
            return;
        }
    }
    m_OutputFile = std::ofstream{ m_DefaultOutputFile };
}
