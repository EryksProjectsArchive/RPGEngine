/*****************************************************
*
* RPG - 2d game project
* File: Graphics/Font.cpp
* Author: Eryk Dwornicki
*
*****************************************************/

#include "Font.h"

#include <Logger.h>

#include <cctype>

#include <stdarg.h>
#include <stdio.h>

#include <Graphics/Graphics.h>

Font::Font(const char *fileName)
	: m_sprite(NULL)
{
	FILE *fp = fopen(fileName, "rb");
	if (fp)
	{
		char texture[256] = { 0 };
		char line[256] = { 0 };
		char *p = NULL;
		Vector2d spriteSize;
		while (fgets(line, 256, fp))
		{
			p = line;
			if (*p == '#') // Skip comments
				continue;

			if (sscanf(line, "Texture %s", texture) != 1)
			{
				unsigned char c = 0;
				unsigned int x = 0;
				unsigned int y = 0;
				unsigned int w = 0;
				unsigned int h = 0;
				if (sscanf(line, "%c %d %d %d %d", &c, &x, &y, &w, &h) == 5)
				{
					if (m_sprite)
					{					
						m_data[c].x = ((float)x / spriteSize.x);
						m_data[c].y = 1.0f - ((float)y / spriteSize.y);
						m_data[c].w = ((float)(x+w) / spriteSize.x);
						m_data[c].h = 1.0f - ((float)(y+h) / spriteSize.y);						
						m_data[c].isSet = true;
					}
					else
					{
						Error("[font] Glyph definition found but no texture has been set. '%s'", fileName);
					}
				}
			}
			else
			{
				char texturePath[256] = { 0 };
				sprintf(texturePath, "../Data/Fonts/%s", texture);
				m_sprite = new Sprite(texturePath);
				spriteSize = m_sprite->GetSize();
			}
		}

		if (!m_sprite)
		{
			Error("[font] No sprite set '%s'.", fileName);
		}

		fclose(fp);
	}
	else
	{
		Error("[font] Unable to create font '%s'.", fileName);
	}
}

Font::~Font()
{
	if (m_sprite)
	{
		delete m_sprite;
		m_sprite = NULL;
	}
}

void Font::Draw(Graphics* graphics, const Vector2d& pos, const Color& color, const char *string, ...)
{
	if (!m_sprite)
		return;

	char buffer[512] = { 0 };
	va_list args;
	va_start(args, string);
	vsprintf(buffer, string, args);
	va_end(args);

	Vector2d position = pos;
	Vector2d size = m_sprite->GetSize();
	for (int i = 0; i < strlen(buffer); ++i)
	{
		char c = toupper(buffer[i]); // TODO: Specify in font descriptor if we want to uppercase each character or perform any other operation.
		GlyphData data = m_data[c];
		if (data.isSet)
		{
			graphics->DrawSpritePart(m_sprite, Rectangle(position.x, position.y, (data.w - data.x) * size.x, (data.y - data.h) * size.y), Vector2d(data.x, data.y), Vector2d(data.w, data.h), color);
			position.x += (data.w - data.x) * size.x + 2.0f;
		}
		else
		{
			if (buffer[i] != ' ')
				graphics->DrawRectangle(Rectangle(position.x, position.y, 10, 10), color);
			position.x += 12.f;
		}
	}
}