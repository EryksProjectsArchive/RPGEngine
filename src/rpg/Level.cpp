/*****************************************************
*
* RPG - 2d game project
* File: Level.cpp
* Author: Eryk Dwornicki
*
*****************************************************/

#include "Level.h"

#include "Logger.h"

#include <Graphics/Sprite.h>
#include "Time.h"

#include <stdio.h>

struct Vertex
{
	float x, y;
	unsigned int color;
	float u, v;

	Vertex() { x = y = u = v = 0.0f; color = 0xFFFFFFFF;  }
	Vertex(float _x, float _y, float _u, float _v, float alpha)
	{
		x = _x;
		y = _y;
		u = _u;
		v = _v;
		color = 0x00FFFFFF | (((unsigned int)(255*alpha)) << 24);
	}
};

Level::Level()
{
	m_width = 500;
	m_height = 500;
	m_fadeOut = 20;

	m_tileSize = 128.0f;
	m_tileSprites = new Sprite("../Data/Sprites/LevelTiles.bmp");
	SetupTileDefinition();
	
	m_start = Vector2d(-(float)((m_tileSize * m_width) / 2), -(float)((m_tileSize * m_height) / 2));
	m_bounds.min = Vector2d(m_start.x, m_start.y);
	m_bounds.max = Vector2d(-m_start.x, -m_start.y);

	m_matrix = glm::translate(glm::mat4(), glm::vec3((int)m_start.x, (int)m_start.y, 0));

	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_width * m_height * 4, NULL, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_width * m_height * 6, NULL, GL_STATIC_DRAW);
	
	Vertex* vertexBuffer = (Vertex *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	unsigned int *indexBuffer = (unsigned int *)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);

	m_tiles = new Tile*[m_width];
	for (int x = 0; x < m_width; ++x)
		m_tiles[x] = new Tile[m_height];
		
	Vector2d tilePos;
	unsigned int index = 0;
	unsigned int i = 0;
	for (unsigned int y = 0; y < m_height; ++y)
	{
		for (unsigned int x = 0; x < m_width; ++x)
		{
			unsigned int XX = rand() % 20;
			TileType type = XX == 2 ? TILE_TYPE_DIRT : TILE_TYPE_GRASS;
			switch (XX)
			{
			case 1:
				type = TILE_TYPE_DIRT;
				break;
			case 2:
				type = TILE_TYPE_ICE;
				break;
			case 3:
			case 4:
				type = TILE_TYPE_GRASS_2;
				break;
			case 5:
			case 6:
				type = TILE_TYPE_GRASS_3;
				break;
			case 7:
				type = TILE_TYPE_SAND;
				break;
			default:
				type = TILE_TYPE_GRASS;
				break;
			}

			m_tiles[x][y].type = type;
			m_tiles[x][y].pos = Vector2d(tilePos.x, tilePos.y);

			float xalpha = 1.0f;
			float yalpha = 1.0f;
			if (x < m_fadeOut)
			{
				xalpha = 1.0f - ((float)m_fadeOut - (float)(x + 1)) / m_fadeOut;
			}
			
			if (y < m_fadeOut)
			{
				yalpha = 1.0f - ((float)m_fadeOut - (float)(y + 1)) / m_fadeOut;
			}
			
			if (x > (m_width - m_fadeOut))
			{
				xalpha = 1.0f - ((float)m_fadeOut - (float)((m_width - x) + 1)) / m_fadeOut;
			}

			if (y > (m_height - m_fadeOut))
			{
				yalpha = 1.0f - ((float)m_fadeOut - (float)((m_height - y) + 1)) / m_fadeOut;
			}

			float alpha = (xalpha * yalpha);

			TileDefinition def = m_tileDefs[type];

			unsigned int a = index++;
			*vertexBuffer++ = Vertex(tilePos.x, tilePos.y, def.x, def.y, alpha);
			unsigned int b = index++;
			*vertexBuffer++ = Vertex(tilePos.x, tilePos.y + m_tileSize, def.x, def.h, alpha);
			unsigned int c = index++;
			*vertexBuffer++ = Vertex(tilePos.x + m_tileSize, tilePos.y, def.w, def.y, alpha);
			unsigned int d = index++;
			*vertexBuffer++ = Vertex(tilePos.x + m_tileSize, tilePos.y + m_tileSize, def.w, def.h, alpha);
			
			*indexBuffer++ = a;
			*indexBuffer++ = b;
			*indexBuffer++ = c;
			*indexBuffer++ = b;
			*indexBuffer++ = c;
			*indexBuffer++ = d;

			tilePos.x += m_tileSize;
			
		}
		tilePos.x = 0.0f;
		tilePos.y += m_tileSize;
	}

	glUnmapBuffer(GL_ARRAY_BUFFER);
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
}

Level::~Level()
{
	for (int x = 0; x < m_width; ++x)
		delete[] m_tiles[x];

	delete []m_tiles;


	if (m_tileSprites)
	{
		delete m_tileSprites;
		m_tileSprites = NULL;
	}	
}

void Level::SetupTileDefinition()
{
	Vector2d vec = m_tileSprites->GetSize();
	
	// NOTE: 1 pixel padding is added to affect some Floating-point artifacts on tile textures.
	float onePixelX = 1.0f / vec.x;
	float onePixelY = 1.0f / vec.y;

	Vector2d pos;
	for (unsigned int i = 0; i < TILE_TYPE_COUNT; ++i)
	{
		m_tileDefs[i].x = (pos.x + onePixelX)/ vec.x;
		m_tileDefs[i].y = 1.0f - ((pos.y + onePixelY) / vec.y);
		m_tileDefs[i].w = ((pos.x + m_tileSize) - onePixelX) / vec.x;
		m_tileDefs[i].h = 1.0f - (((pos.y + m_tileSize) - onePixelY) / vec.y);

		pos.x += m_tileSize;
		if (pos.x >= vec.x)
		{
			pos.x = 0.0f;
			pos.y += m_tileSize;
		}
	}
}

LevelBounds Level::GetBounds()
{
	return m_bounds;
}

void Level::Render(Graphics& graphics)
{	
	graphics.DrawTileset(m_vbo, m_ibo, m_width * m_height * 6, m_tileSprites, m_matrix);
}

TileType Level::GetTypeAt(const Vector2d& position)
{
	int x = (int)((position.x - m_start.x) / m_tileSize);
	int y = (int)((position.y - m_start.y) / m_tileSize);

	if (x < 0 || x > m_width || y < 0 || y > m_height)
		return TILE_TYPE_EMPTY;

	return m_tiles[x][y].type;
}