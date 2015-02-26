/*****************************************************
*
* RPG - 2d game project
* File: Sprite/Sprite.h
* Author: Eryk Dwornicki
*
*****************************************************/

#pragma once

#include <Math/Vector2d.h>

class Sprite
{
protected:
	Vector2d m_size;
	unsigned int m_textureId;
	float m_opacity;

	Sprite();
public:	
	Sprite(const char *filePath);
	virtual ~Sprite();

	virtual Vector2d GetSize();

	virtual unsigned int GetTextureId();

	virtual void SetOpacity(float opacity);
	virtual float GetOpacity();
};