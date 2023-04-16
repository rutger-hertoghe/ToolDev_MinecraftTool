#include <chrono>
#include <iostream>

#include "rapidjson/istreamwrapper.h"
#include "rapidjson/document.h"

#include "Chunk.h"
#include "FileHandler.h"

int wmain(int argc, wchar_t* argv[])
{
    const auto pFileHandler{ std::make_unique<FileHandler>(argc, argv) };
    if (!pFileHandler->IsValid())
    {
        return 1;
    }

    rapidjson::IStreamWrapper inputFileWrapper{ pFileHandler->GetInputFile() };
    rapidjson::Document jsonDoc;
    jsonDoc.ParseStream(inputFileWrapper);

    const auto pChunk{ std::make_unique<Chunk>() };

    pChunk->LoadFromJson(jsonDoc);
    pChunk->WriteChunkObj(pFileHandler->GetOutputFile());

    return 0;
 }