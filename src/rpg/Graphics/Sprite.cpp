/*****************************************************
*
* RPG - 2d game project
* File: Sprite/Sprite.cpp
* Author: Eryk Dwornicki
*
*****************************************************/

#include "Sprite.h"

#include <stdio.h>

#include <Graphics/Graphics.h>
#include <Logger.h>

struct Magic
{
	unsigned char id[2];  // id must be "BM"
};

struct Header
{
	unsigned int size; // size of the whole bmp file
	unsigned int reserved; // must be 0
	unsigned int offBits;
};

struct InfoHeader
{
	unsigned long size;
	long width;
	long height;
	unsigned short planes;
	unsigned short bitCount;
	unsigned long compression;
	unsigned long sizeImage;
	long xPelsPerMeter;
	long yPelsPerMeter;
	unsigned long clrUsed;
	unsigned long clrImportant;
};


#define CATCH_ERROR\
		{unsigned int error = glGetError();\
	if (error != 0) {\
		Error("[graphics] %s:%d %d:%s", __FILE__, __LINE__, error, gluErrorString(error));\
				}}

Sprite::Sprite()
{
	m_textureId = -1;
	m_size = Vector2d(1, 1);
	m_opacity = 1.0f;
}

Sprite::Sprite(const char *fileName)
{
	m_textureId = -1;
	m_size = Vector2d(50, 50);
	m_opacity = 1.0f;

	Info("[sprite] %s", fileName);
	FILE *fp = fopen(fileName, "rb");
	if (fp)
	{
		Magic magic = { 0 };
		fread(&magic, sizeof(Magic), 1, fp);

		if (magic.id[0] != 'B' || magic.id[1] != 'M')
		{
			Error("[sprite] Invalid bmp signature");
			return;
		}
		
		Header header = { 0 };
		fread(&header, sizeof(Header), 1, fp);

		if (header.size > 0)
		{
			InfoHeader infoHeader;
			fread(&infoHeader, sizeof(InfoHeader), 1, fp);

			if (infoHeader.bitCount == 24 && infoHeader.compression == 0)
			{
				m_size = Vector2d((float)infoHeader.width, (float)infoHeader.height);
				unsigned int size = infoHeader.width * infoHeader.height * 3;

				unsigned char *rawPixels = new unsigned char[size];
				unsigned char *rawPixelsPtr = rawPixels;
				fseek(fp, header.offBits, SEEK_SET);

				fread(rawPixels, sizeof(unsigned char), size, fp);
				
				unsigned char alphaKey[3] = { 51, 255, 0 };
				unsigned char *pixels = new unsigned char[infoHeader.width * infoHeader.height * 4];
				unsigned char *p = pixels;
				for (unsigned int i = 0; i < infoHeader.width * infoHeader.height; ++i)
				{
					unsigned char b = *rawPixelsPtr++;
					unsigned char g = *rawPixelsPtr++;
					unsigned char r = *rawPixelsPtr++;
					*p++ = r;
					*p++ = g;
					*p++ = b;
					*p++ = (r == alphaKey[0] && g == alphaKey[1] && b == alphaKey[2]) ? 0x00 : 0xFF;
				}
								
				glGenTextures(1, &m_textureId);
				glBindTexture(GL_TEXTURE_2D, m_textureId);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, infoHeader.width, infoHeader.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);


				CATCH_ERROR;
				delete[]pixels;
				delete[]rawPixels;
			}
			else
			{
				Warning("[sprite] Unsupported file type BC %d C %d", infoHeader.bitCount, infoHeader.compression);
			}
		}
		fclose(fp);
	}
	else
	{
		Error("[sprite] Unable to open file %s", fileName);
	}
}

Sprite::~Sprite()
{
}

Vector2d Sprite::GetSize()
{
	return m_size;
}

unsigned int Sprite::GetTextureId()
{
	return m_textureId;
}

void Sprite::SetOpacity(float opacity)
{
	m_opacity = opacity;
}

float Sprite::GetOpacity()
{
	return m_opacity;
}