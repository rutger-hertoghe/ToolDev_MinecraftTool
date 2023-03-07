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

	std::vector<Position> m_ChunkVertices;
	std::vector<Face> m_ChunkFaces;

	std::vector<Position> GenerateBlockVertices(Block* pBlock);
	std::vector<Face> ConstructFaces(Block* pBlock);
	void AddOptimizedFacesAndVerts(Face& face, const std::vector<Position>& verts);
	void OptimizeVertex(int& vertexIdx, const std::vector<Position>& verts); // TODO: rename?

	//void WriteFace(std::ofstream& outputFile, int blockIdx, int normalIdx, int v0, int v1, int v2);
	//void WriteFaces(std::ofstream& outputFile, Block* pBlock, int blockIdx);
	void WriteFaces(std::ofstream& outputFile) const;

	//void WriteVertex(std::ofstream& outputFile, const Position& pos, int x, int y, int z);
	//void WriteVertices(std::ofstream& outputFile, const Position& pos);
	void WriteVertices(std::ofstream& outputFile) const;
};

