#include <stdexcept>
#include <fstream>

#include "Chunk.h"

#include <algorithm>

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
    // INFO
    outputFile << "#.json to .obj converter by Rutger Hertoghe\n\n";
    outputFile << "mtllib minecraftMats.mtl\n\n";

    // CREATE & OPTIMIZE FACES
    int i{ 0 }; // Work around for correct indexing of face vertices with unordered_map
    for (const auto& elem : m_Blocks)
    {
        const auto pBlock = elem.second;
        auto pos = pBlock->GetPosition();

        auto verts = GenerateBlockVertices(pBlock);
        auto faces = ConstructFaces(pBlock);

        for (auto& face : faces)
        {
            OptimizeVertex(face.v0, verts);
            OptimizeVertex(face.v1, verts);
            OptimizeVertex(face.v2, verts);
            OptimizeVertex(face.v3, verts);

            m_ChunkFaces.emplace_back(face);
        }
        ++i;
    }

    SortFacesByMaterial();

    // WRITE OBJ
    WriteVertices(outputFile);
    WriteNormals(outputFile);
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
    outputFile << "\n";
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
    auto mat{ pBlock->GetMaterial() };

    std::vector<Face> faces;
    if (!HasNeighborInDirection(pBlock, 0, 0, -1))
    {
        constexpr int normalIndex{ 2 };
        faces.emplace_back(Face{ 1, 3, 7, 5, normalIndex, mat});
    }

    // On X-axis
    if (!HasNeighborInDirection(pBlock, -1, 0, 0))
    {
        constexpr int normalIndex{ 6 };
        faces.emplace_back(Face{ 1, 2, 4, 3, normalIndex, mat });
    }

    // Y+
    if (!HasNeighborInDirection(pBlock, 0, 1, 0))
    {
        constexpr int normalIndex{ 3 };
        faces.emplace_back(Face{ 3, 4, 8, 7, normalIndex, mat });
    }

    // On X-axis
    if (!HasNeighborInDirection(pBlock, 1, 0, 0))
    {
        constexpr int normalIndex{ 5 };
        faces.emplace_back(Face{ 5, 7, 8, 6, normalIndex, mat });
    }

    // Y-
    if (!HasNeighborInDirection(pBlock, 0, -1, 0))
    {
        constexpr int normalIndex{ 4 };
        faces.emplace_back(Face{ 1, 5, 6, 2, normalIndex, mat });
    }

    // On Z-axis
    if (!HasNeighborInDirection(pBlock, 0, 0, 1))
    {
        constexpr int normalIndex{ 1 };
        faces.emplace_back(Face{ 2, 6, 8, 4, normalIndex, mat });
    }

    return faces;
}

void Chunk::OptimizeVertex(int& vertexIdx, const std::vector<Position>& verts)
{
    const int correctedIndex{ vertexIdx - 1 };

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

void Chunk::SortFacesByMaterial()
{
    std::sort(m_ChunkFaces.begin(), m_ChunkFaces.end(),
        [](const Face& left, const Face& right)
        {
            return static_cast<int>(left.material) < static_cast<int>(right.material);
        });
}

bool Chunk::IsDifferentMaterial(Material& currentMaterial, Material faceMaterial) const
{
    if (currentMaterial == faceMaterial) return false;

    currentMaterial = faceMaterial;
    return true;
}

void Chunk::WriteMaterial(std::ofstream& outputFile, Material currentMaterial) const
{
    outputFile << "\nusemtl ";
    switch(currentMaterial)
    {
    case Material::dirt:
        outputFile << "Dirt";
        break;
    case Material::glass:
        outputFile << "Glass";
        break;
    case Material::stone:
        outputFile << "Stone";
        break;
    case Material::wood:
        outputFile << "Wood";
        break;
    default:
        outputFile << "Invalid";
    }
    outputFile << "\n";
}

void Chunk::WriteFaces(std::ofstream& outputFile) const
{
    Material currentMaterial{Material::ENUM_END}; // Initialized with ENUM_END to guarantee material writing on first material

    for(const auto& face : m_ChunkFaces)
    {
        if(IsDifferentMaterial(currentMaterial, face.material))
        {
        	WriteMaterial(outputFile, currentMaterial);
        }

        outputFile << "f "
            << face.v0 << "//" << face.vn << " "
            << face.v1 << "//" << face.vn << " "
            << face.v2 << "//" << face.vn << " "
            << face.v3 << "//" << face.vn << "\n";
    }
}

void Chunk::WriteVertices(std::ofstream& outputFile) const
{
    for(const auto& vertex : m_ChunkVertices)
    {
        outputFile << "v " << vertex.x << " " << vertex.y << " " << vertex.z << "\n";
    }
    outputFile << "\n";
}
