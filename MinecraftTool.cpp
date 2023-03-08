#include <chrono>
#include <iostream>

#include "rapidjson/istreamwrapper.h"
#include "rapidjson/document.h"

#include "Chunk.h"
#include "FileHandler.h"
#include <numeric>

int wmain(int argc, wchar_t* argv[], wchar_t* envp[])
{
    constexpr int sampleSize{ 1000 };
    std::vector<float> m_RawPtrMeasurements;
    for(int i{0}; i < sampleSize; ++i)
    {
        const auto start{ std::chrono::high_resolution_clock::now() };

        //const auto pFileHandler{ std::make_unique<FileHandler>(argc, argv) };
        FileHandler* pFileHandler{ new FileHandler{ argc, argv } };

        rapidjson::IStreamWrapper inputFileWrapper{ pFileHandler->GetInputFile() };
        rapidjson::Document jsonDoc;
        jsonDoc.ParseStream(inputFileWrapper);

        //const auto pChunk{ std::make_unique<Chunk>() };
        Chunk* pChunk{ new Chunk{} };

        pChunk->LoadFromJson(jsonDoc);

        pChunk->WriteChunkObj(pFileHandler->GetOutputFile());

        delete pFileHandler;
        delete pChunk;

        const auto end{ std::chrono::high_resolution_clock::now() };
        const auto duration{ std::chrono::duration<float>(end - start).count() };
        m_RawPtrMeasurements.emplace_back(duration);
    }
    auto sum{ std::accumulate(m_RawPtrMeasurements.begin(), m_RawPtrMeasurements.end(), 0.f) };
    auto mean{ sum / static_cast<float>(m_RawPtrMeasurements.size()) };
    float sumOfSqDeviations{0};
    for(auto val : m_RawPtrMeasurements)
    {
        auto diff{ mean - val };
        auto sqDiff{ diff * diff };
        sumOfSqDeviations += sqDiff;
    }
    float stdDeviation{ sqrtf(sumOfSqDeviations / sampleSize) };

    std::cout << "Using raw pointers results in a mean running time of: " << mean << '\n'
        << "and a standard deviation of: " << stdDeviation << '\n';


    std::vector<float> m_SmartPtrMeasurements;
    for (int i{ 0 }; i < sampleSize; ++i)
    {
        const auto start{ std::chrono::high_resolution_clock::now() };

        const auto pFileHandler{ std::make_unique<FileHandler>(argc, argv) };
        //FileHandler* pFileHandler{ new FileHandler{ argc, argv } };

        rapidjson::IStreamWrapper inputFileWrapper{ pFileHandler->GetInputFile() };
        rapidjson::Document jsonDoc;
        jsonDoc.ParseStream(inputFileWrapper);

        const auto pChunk{ std::make_unique<Chunk>() };
        //Chunk* pChunk{ new Chunk{} };

        pChunk->LoadFromJson(jsonDoc);

        pChunk->WriteChunkObj(pFileHandler->GetOutputFile());

        //delete pFileHandler;
        //delete pChunk;

        const auto end{ std::chrono::high_resolution_clock::now() };
        const auto duration{ std::chrono::duration<float>(end - start).count() };
        m_SmartPtrMeasurements.emplace_back(duration);
    }
    sum = std::accumulate(m_SmartPtrMeasurements.begin(), m_SmartPtrMeasurements.end(), 0.f);
    mean = sum / static_cast<float>(m_SmartPtrMeasurements.size());
    sumOfSqDeviations = 0;
    for (auto val : m_RawPtrMeasurements)
    {
        auto diff{ mean - val };
        auto sqDiff{ diff * diff };
        sumOfSqDeviations += sqDiff;
    }
    stdDeviation = sqrtf(sumOfSqDeviations / sampleSize);

    std::cout << "Using smart pointers results in a mean running time of: " << mean << '\n'
        << "and a standard deviation of: " << stdDeviation << '\n';


    //return 0;
 }