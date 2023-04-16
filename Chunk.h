#ifndef CHUNK_H
#define CHUNK_H
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

	bool FaceNeedsRendering(Block* pBlock, int faceDirectionX, int faceDirectionY, int faceDirectionZ);
	Block* GetNeighborInDirection(Block* pBlock, int xDir, int yDir, int zDir);

	void WriteNormals(std::ofstream& outputFile);

	std::vector<Position> m_ChunkVertices;
	std::vector<Face> m_ChunkFaces;

	std::vector<Position> GenerateBlockVertices(Block* pBlock);
	std::vector<Face> ConstructFaces(Block* pBlock);

	// If vertex already exists in m_ChunkVertices, face uses that vertex instead (prevent duplicates), otherwise vertex is added to vertex container
	void OptimizeVertex(int& vertexIdx, const std::vector<Position>& verts);

	void SortFacesByMaterial();
	bool IsDifferentMaterial(Material& currentMaterial, Material faceMaterial) const;
	void WriteMaterial(std::ofstream& outputFile, Material currentMaterial) const;

	void WriteFaces(std::ofstream& outputFile) const;

	void WriteVertices(std::ofstream& outputFile) const;
};

#endif