#include "rapidjson/istreamwrapper.h"
#include "rapidjson/document.h"

#include "Chunk.h"
#include "FileHandler.h"

int wmain(int argc, wchar_t* argv[], wchar_t* envp[])
{
    FileHandler* pFileHandler{ new FileHandler{ argc, argv } };

    rapidjson::IStreamWrapper inputFileWrapper{ pFileHandler->GetInputFile() };
    rapidjson::Document jsonDoc;
    jsonDoc.ParseStream(inputFileWrapper);

    Chunk* pChunk{new Chunk{}};

    pChunk->LoadFromJson(jsonDoc);

    pChunk->WriteChunkObj(pFileHandler->GetOutputFile());

    delete pChunk;
    delete pFileHandler;
    //return 0;
 }