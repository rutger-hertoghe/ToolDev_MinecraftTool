#pragma once
#include <fstream>

class FileHandler final
{
public:
	explicit FileHandler(int argc, wchar_t* argv[], const std::string& defaultOutputFile = "defaultScene.obj");

	std::ifstream& GetInputFile();
	std::ofstream& GetOutputFile();

private:
	std::ifstream m_InputFile;
	std::ofstream m_OutputFile;

	std::string m_DefaultOutputFile;

	void CheckForInputFile(int argc, wchar_t* argv[]);
	void CheckForOutputFile(int argc, wchar_t* argv[]);
};

