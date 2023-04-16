#include "FileHandler.h"

#include <iostream>

class InputFileNotFound{};

FileHandler::FileHandler(int argc, wchar_t* argv[], const std::string& defaultOutputFile )
	: m_DefaultOutputFile{defaultOutputFile}
	, m_IsValid{false}
{
    m_IsValid = CheckForInputFile(argc, argv);
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

bool FileHandler::IsValid()
{
    return m_IsValid;
}

bool FileHandler::CheckForInputFile(int argc, wchar_t* argv[])
{
    for (int i{ 0 }; i < argc; ++i)
    {
        if (static_cast<std::wstring>(argv[i]) == L"-i" && i + 1 < argc)
        {
            m_InputFile = std::ifstream{ argv[i + 1] };
            return true;
        }
    }
    std::cout << "No input file specified! Program was not executed!\n";
    return false;
}

bool FileHandler::CheckForOutputFile(int argc, wchar_t* argv[])
{
    for (int i{ 0 }; i < argc; ++i)
    {
        if (static_cast<std::wstring>(argv[i]) == L"-o" && i + 1 < argc)
        {
            m_OutputFile = std::ofstream{ argv[i + 1] };
            if(m_OutputFile) return true;
            std::cout << "Output file was invalid!\n";
            return false;
        }
    }
    std::cout << "No output file specified! Using file with name " << m_DefaultOutputFile << " as output file!\n";
    m_OutputFile = std::ofstream{ m_DefaultOutputFile };
    return false;
}
