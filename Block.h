#pragma once
#include <string>

#include "Structs.h"
enum class Material
{
	dirt,
	glass,
	stone,
	wood,
	ENUM_END
};

class Block final
{
public:
	explicit Block(std::string material, Position position, bool opacity);

	Position GetPosition();
	uint32_t GetHashKey();
	uint32_t GetRelativeHashKey(int xOffset, int yOffset, int zOffset);

private:
	Position m_Position;
	bool m_Opacity;
	Material m_Material;
	uint32_t m_HashKey;

	bool SetMaterial(std::string material);
	void GenerateHashKey();
};

