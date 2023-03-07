#include <stdexcept>
#include <fstream>

#include "Chunk.h"
#include "Block.h"

// TODO: check whether ordered map is a better alternative here?
// TODO: implement materials

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
        const auto pBlock = elem.second;
        auto pos = pBlock->GetPosition();

        auto verts = GenerateBlockVertices(pBlock); // Possibly used for optimization of final output.obj later on
        auto faces = ConstructFaces(pBlock);

        for (auto& face : faces)
        {
            AddOptimizedFacesAndVerts(face, verts);
        }
        ++i;
    }

    WriteVertices(outputFile);
    outputFile << "\n";

    WriteFaces(outputFile);
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

std::vector<Position> Chunk::GenerateBlockVertices(Block* block)
{
    std::vector<Position> vertices;
    const auto pos = block->GetPosition();

    for(int i{0}; i <= 1; ++i)
    {
	    for(int j{0}; j <= 1; ++j)
	    {
		    for(int k{0}; k <= 1; ++k)
		    {
                vertices.push_back(Position{ pos.x + i, pos.y + j, pos.z + k });
		    }
	    }
    }

    return vertices;
}

std::vector<Face> Chunk::ConstructFaces(Block* pBlock)
{
    std::vector<Face> faces;
    if (!HasNeighborInDirection(pBlock, 0, 0, -1))
    {
        constexpr int normalIndex{ 2 };
        faces.emplace_back(Face{ 1, 7, 5, normalIndex });
        faces.emplace_back(Face{ 1, 3, 7, normalIndex });
    }

    // On X-axis
    if (!HasNeighborInDirection(pBlock, -1, 0, 0))
    {
        constexpr int normalIndex{ 6 };
        faces.emplace_back(Face{ 1, 4, 3, normalIndex });
        faces.emplace_back(Face{ 1, 2, 4, normalIndex });
    }

    // Y+
    if (!HasNeighborInDirection(pBlock, 0, 1, 0))
    {
        constexpr int normalIndex{ 3 };
        faces.emplace_back(Face{ 3, 8, 7, normalIndex });
        faces.emplace_back(Face{ 3, 4, 8, normalIndex });
    }

    // On X-axis
    if (!HasNeighborInDirection(pBlock, 1, 0, 0))
    {
        constexpr int normalIndex{ 5 };
        faces.emplace_back(Face{ 5, 7, 8, normalIndex });
        faces.emplace_back(Face{ 5, 8, 6, normalIndex });
    }

    // Y-
    if (!HasNeighborInDirection(pBlock, 0, -1, 0))
    {
        constexpr int normalIndex{ 4 };
        faces.emplace_back(Face{ 1, 5, 6, normalIndex });
        faces.emplace_back(Face{ 1, 6, 2, normalIndex });
    }

    // On Z-axis
    if (!HasNeighborInDirection(pBlock, 0, 0, 1))
    {
        constexpr int normalIndex{ 1 };
        faces.emplace_back(Face{ 2, 6, 8, normalIndex });
        faces.emplace_back(Face{ 2, 8, 4, normalIndex });
    }

    return faces;
}

void Chunk::AddOptimizedFacesAndVerts(Face& face, const std::vector<Position>& verts)
{
    OptimizeVertex(face.v0, verts);
    OptimizeVertex(face.v1, verts);
    OptimizeVertex(face.v2, verts);

    m_ChunkFaces.emplace_back(face);
}

void Chunk::OptimizeVertex(int& vertexIdx, const std::vector<Position>& verts)
{
    int correctedIndex{ vertexIdx - 1 };

    auto it = std::find(begin(m_ChunkVertices), end(m_ChunkVertices), verts[correctedIndex]);
    if (it != end(m_ChunkVertices))
    {
        vertexIdx = static_cast<int>(it - m_ChunkVertices.begin() + 1); // Should get correct index
    }
    else
    {
        m_ChunkVertices.emplace_back(verts[correctedIndex]);
        vertexIdx = static_cast<int>(m_ChunkVertices.size());
    }
}

void Chunk::WriteFaces(std::ofstream& outputFile) const
{
    for(const auto& face : m_ChunkFaces)
    {
        outputFile << "f "
            << face.v0 << "//" << face.vn << " "
            << face.v1 << "//" << face.vn << " "
            << face.v2 << "//" << face.vn << "\n";
    }
}

void Chunk::WriteVertices(std::ofstream& outputFile) const
{
    for(const auto& vertex : m_ChunkVertices)
    {
        outputFile << "v " << vertex.x << " " << vertex.y << " " << vertex.z << "\n";
    }
}
