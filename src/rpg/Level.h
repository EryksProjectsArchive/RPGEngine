/*****************************************************
*
* RPG - 2d game project
* File: Level.h
* Author: Eryk Dwornicki
*
*****************************************************/

#pragma once

#include <ForwardDecls.h>
#include <Graphics/Graphics.h>

enum TileType
{	
	TILE_TYPE_EMPTY,
	TILE_TYPE_GRASS,
	TILE_TYPE_DIRT,
	TILE_TYPE_ICE,
	TILE_TYPE_COUNT
};

struct LevelBounds
{
	Vector2d min;
	Vector2d max;
};

class Level
{
private:
	struct Tile
	{
		TileType type;
		Vector2d pos;
		Tile()
		{
		}
	};
	Tile **m_tiles;

	struct TileDefinition
	{
		float x, y, w, h;
	} m_tileDefs[TILE_TYPE_COUNT];

	unsigned int m_vbo;
	unsigned int m_ibo;

	float m_tileSize;

	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_fadeOut;

	glm::mat4 m_matrix;
	Vector2d m_start;
	
	Sprite* m_tileSprites;

	LevelBounds m_bounds;
public:
	Level();
	~Level();

	void SetupTileDefinition();

	LevelBounds GetBounds();

	TileType GetTypeAt(const Vector2d& position);

	void Render(Graphics& graphics);
};