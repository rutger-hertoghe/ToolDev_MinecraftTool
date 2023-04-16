#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <fstream>

class FileHandler final
{
public:
	explicit FileHandler(int argc, wchar_t* argv[], const std::string& defaultOutputFile = "defaultScene.obj");

	std::ifstream& GetInputFile();
	std::ofstream& GetOutputFile();

	bool IsValid();

private:
	bool m_IsValid;

	std::ifstream m_InputFile;
	std::ofstream m_OutputFile;

	std::string m_DefaultOutputFile;

	bool CheckForInputFile(int argc, wchar_t* argv[]);
	bool CheckForOutputFile(int argc, wchar_t* argv[]);
};

#endif