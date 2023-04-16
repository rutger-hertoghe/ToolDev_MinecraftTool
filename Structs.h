#ifndef STRUCT_H
#define STRUCT_H

enum class Material
{
	dirt,
	glass,
	stone,
	wood,
	ENUM_END
};

struct Position
{
	bool operator==(const Position& other) const
	{
		if (x != other.x) return false;
		if (y != other.y) return false;
		if (z != other.z) return false;
		return true;
	}
	int x;
	int y;
	int z;
};

struct Face
{
	int v0;
	int v1;
	int v2;
	int v3;

	int vn;
	Material material;
};
#endif