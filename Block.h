#ifndef BLOCK_H
#define BLOCK_H

#include <string>

#include "Structs.h"

class Block final
{
public:
	explicit Block(std::string material, Position position, bool opacity);

	Position GetPosition() const;
	uint32_t GetHashKey() const;
	uint32_t GetRelativeHashKey(int xOffset, int yOffset, int zOffset) const;
	Material GetMaterial() const;

private:
	Position m_Position;
	bool m_Opacity;
	Material m_Material;
	uint32_t m_HashKey;

	bool SetMaterial(std::string material);
	void GenerateHashKey();
};
#endif