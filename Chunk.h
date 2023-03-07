#pragma once
#include <unordered_map>

#include "Structs.h"
#include "rapidjson/document.h"

class Block;

class Chunk final
{
public:
	Chunk() = default;
	~Chunk();

	Chunk(const Chunk& other) = delete;
	Chunk(Chunk&& other) = delete;
	Chunk& operator=(const Chunk& other) = delete;
	Chunk& operator=(Chunk&& other) = delete;

	void LoadFromJson(const rapidjson::Document& jsonDoc);
	void WriteChunkObj(std::ofstream& outputFile);

private:
	std::unordered_map<uint32_t, Block*> m_Blocks{};

	void AddBlock(Block* block);

	bool HasNeighborInDirection(Block* pBlock, int xDir, int yDir, int zDir);

	void WriteNormals(std::ofstream& outputFile);

	void WriteFace(std::ofstream& outputFile, const int& blockIdx, const int& normalIdx, int v0, int v1, int v2);
	void WriteFaces(std::ofstream& outputFile, Block* block, const int& blockIdx);

	void WriteVertex(std::ofstream& outputFile, const Position& pos, int x, int y, int z);
	void WriteVertices(std::ofstream& outputFile, const Position& pos);
};

