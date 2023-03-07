#include <stdexcept>
#include <fstream>

#include "Chunk.h"
#include "Block.h"

// TODO: check whether ordered map is a better alternative here?
// TODO: look into optimization: specifically, remove duplicate vertices & replace their index in the face writing by the one already in the list
// Possible solution: use 

Chunk::~Chunk()
{
	for(const auto& block : m_Blocks)
	{
		delete block.second;
	}
}

void Chunk::AddBlock(Block* block)
{
	m_Blocks.emplace(block->GetHashKey(), block);
}

void Chunk::LoadFromJson(const rapidjson::Document& jsonDoc)
{
    for (rapidjson::SizeType i = 0; i < jsonDoc.Size(); i++) 
    {
        const rapidjson::Value& object = jsonDoc[i];

        // MATERIAL
        const char* layer{ object["layer"].GetString() };
        std::string material{ layer };

        // Remove whitespace from material string
        material.erase(std::remove_if(material.begin(), material.end(), ::isspace), material.end());

        // OPACITY
        const bool opacity{ object["opaque"].GetBool() };

        // POSITIONS
        const rapidjson::Value& positions = object["positions"];

        // Iterate through the array of positions
        for (rapidjson::SizeType j = 0; j < positions.Size(); j++) 
        {
            const rapidjson::Value& position = positions[j];

            Position pos{};
            pos.x = position[0].GetInt();
            pos.y = position[2].GetInt();
            pos.z = position[1].GetInt();

            AddBlock(new Block{ material, pos, opacity });
        }
    }
}

void Chunk::WriteChunkObj(std::ofstream& outputFile)
{
    WriteNormals(outputFile);
    outputFile << "\n";

    int i{ 0 }; // Work around for correct indexing of face vertices with unordered_map
    for (const auto& elem : m_Blocks)
    {
        auto block = elem.second;
        auto pos = block->GetPosition();

        WriteVertices(outputFile, pos);
        outputFile << "\n";

        WriteFaces(outputFile, block, i);

        ++i;
    }
}

bool Chunk::HasNeighborInDirection(Block* pBlock, int xDir, int yDir, int zDir)
{
    const auto key{ pBlock->GetRelativeHashKey(xDir, yDir, zDir) };
    try
    {
        auto val = m_Blocks.at(key);
    }
    catch (std::out_of_range&)
    {
        return false;
    }
    return true;
}

void Chunk::WriteNormals(std::ofstream& outputFile)
{
    outputFile << "vn " <<  0.0f << " " <<  0.0f << " " <<  1.0f << "\n";
    outputFile << "vn " <<  0.0f << " " <<  0.0f << " " << -1.0f << "\n";
    outputFile << "vn " <<  0.0f << " " <<  1.0f << " " <<  0.0f << "\n";
    outputFile << "vn " <<  0.0f << " " << -1.0f << " " <<  0.0f << "\n";
    outputFile << "vn " <<  1.0f << " " <<  0.0f << " " <<  0.0f << "\n";
    outputFile << "vn " << -1.0f << " " <<  0.0f << " " <<  0.0f << "\n";
}

void Chunk::WriteFace(std::ofstream& outputFile, const int& blockIdx, const int& normalIdx, int v0, int v1, int v2)
{
    int i{ blockIdx * 8 }; // 8 faces per block, so vertex indices per block will be offset by 8 each time
    outputFile << "f "
        << i + v0 << "//" << normalIdx << " "
        << i + v1 << "//" << normalIdx << " "
        << i + v2 << "//" << normalIdx << "\n";
}

void Chunk::WriteFaces(std::ofstream& outputFile, Block* block, const int& blockIdx)
{
    if (!HasNeighborInDirection(block, 0, 0, -1))
    {
        constexpr int normalIndex{ 2 };
        WriteFace(outputFile, blockIdx, normalIndex, 1, 7, 5);
        WriteFace(outputFile, blockIdx, normalIndex, 1, 3, 7);
    }

    // On X-axis
    if (!HasNeighborInDirection(block, -1, 0, 0))
    {
        constexpr int normalIndex{ 6 };
        WriteFace(outputFile, blockIdx, normalIndex, 1, 4, 3);
        WriteFace(outputFile, blockIdx, normalIndex, 1, 2, 4);
    }

    // Y+
    if (!HasNeighborInDirection(block, 0, 1, 0))
    {
        constexpr int normalIndex{ 3 };
        WriteFace(outputFile, blockIdx, normalIndex, 3, 8, 7);
        WriteFace(outputFile, blockIdx, normalIndex, 3, 4, 8);
    }

    // On X-axis
    if (!HasNeighborInDirection(block, 1, 0, 0))
    {
        constexpr int normalIndex{ 5 };
        WriteFace(outputFile, blockIdx, normalIndex, 5, 7, 8);
        WriteFace(outputFile, blockIdx, normalIndex, 5, 8, 6);
    }

    // Y-
    if (!HasNeighborInDirection(block, 0, -1, 0))
    {
        constexpr int normalIndex{ 4 };
        WriteFace(outputFile, blockIdx, normalIndex, 1, 5, 6);
        WriteFace(outputFile, blockIdx, normalIndex, 1, 6, 2);
    }

    // On Z-axis
    if (!HasNeighborInDirection(block, 0, 0, 1))
    {
        constexpr int normalIndex{ 1 };
        WriteFace(outputFile, blockIdx, normalIndex, 2, 6, 8);
        WriteFace(outputFile, blockIdx, normalIndex, 2, 8, 4);
    }
}

void Chunk::WriteVertex(std::ofstream& outputFile, const Position& pos, int x, int y, int z)
{
    outputFile << "v " << pos.x + x << " " << pos.y + y << " " << pos.z + z << "\n";
}

void Chunk::WriteVertices(std::ofstream& outputFile, const Position& pos)
{
    WriteVertex(outputFile, pos, 0, 0, 0);
    WriteVertex(outputFile, pos, 0, 0, 1);
    WriteVertex(outputFile, pos, 0, 1, 0);
    WriteVertex(outputFile, pos, 0, 1, 1);
    WriteVertex(outputFile, pos, 1, 0, 0);
    WriteVertex(outputFile, pos, 1, 0, 1);
    WriteVertex(outputFile, pos, 1, 1, 0);
    WriteVertex(outputFile, pos, 1, 1, 1);
}
