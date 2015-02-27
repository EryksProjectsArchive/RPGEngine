/*****************************************************
*
* RPG - 2d game project
* File: Graphics/Font.h
* Author: Eryk Dwornicki
*
*****************************************************/

#pragma once

#include <ForwardDecls.h>
#include "Sprite.h"
#include "Color.h"

#include <Math/Vector2d.h>

struct GlyphData
{
	bool isSet;
	float x, y, w, h;

	GlyphData()
	{
		x = y = w = h = 0.0f;
		isSet = false;
	}
};

class Font
{
private:
	GlyphData m_data[256];
	Sprite* m_sprite;
public:
	Font(const char* fileName);
	~Font();

	void Draw(Graphics* graphics, const Vector2d& position, const Color& color, const char *string, ...);
};