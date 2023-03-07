#include "Block.h"

#include <iostream>

class InvalidMaterial{};

Block::Block(std::string material, Position position, bool opacity)
	: m_Opacity{opacity}
	, m_Position{position}
	, m_HashKey{0}
{
	if(!SetMaterial(material))
	{
		std::cout << "Specified material " << material << " was invalid!\n";
		throw InvalidMaterial();
	}

	GenerateHashKey();
}

Position Block::GetPosition()
{
	return m_Position;
}

uint32_t Block::GetHashKey()
{
	return m_HashKey;
}

uint32_t Block::GetRelativeHashKey(int xOffset, int yOffset, int zOffset)
{
	auto x = static_cast<uint8_t>(m_Position.x + xOffset) ;
	auto y = static_cast<uint8_t>(m_Position.y + yOffset) ;
	auto z = static_cast<uint8_t>(m_Position.z + zOffset) ;

	uint32_t hashKey{ 0 };
	hashKey |= x;
	hashKey |= z << 8;
	hashKey |= y << 16;

	return hashKey;
}

bool Block::SetMaterial(std::string material)
{
	if(material == "dirt")
	{
		m_Material = Material::dirt;
		return true;
	}

	if(material == "stone")
	{
		m_Material = Material::stone;
		return true;
	}

	if(material == "wood")
	{
		m_Material = Material::wood;
		return true;
	}

	if(material == "glass")
	{
		m_Material = Material::glass;
		return true;
	}

	m_Material = Material::ENUM_END;
	return false;
}

void Block::GenerateHashKey()
{
	auto x = static_cast<uint8_t>(m_Position.x);
	auto y = static_cast<uint8_t>(m_Position.y);
	auto z = static_cast<uint8_t>(m_Position.z);

	m_HashKey |= x;
	m_HashKey |= z << 8;
	m_HashKey |= y << 16;
}
