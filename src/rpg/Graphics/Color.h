/*****************************************************
*
* RPG - 2d game project
* File: Graphics/Color.h
* Author: Eryk Dwornicki
*
*****************************************************/

#pragma once

class Color
{
public:
	float a;
	float r;
	float g;
	float b;

	Color()
	{
		r = g = b = 0.0f;
		a = 1.0f;
	}

	Color(float _r, float _g, float _b)
	{
		r = _r;
		g = _g;
		b = _b;
		a = 1.0f;
	}

	Color(float _a, float _r, float _g, float _b)
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}

	Color(const Color& color)
	{
		r = color.r;
		g = color.g;
		b = color.b;
		a = color.a;
	}

	~Color()
	{
	}

	unsigned int ToUnsignedInt() const
	{
		unsigned char _r = (unsigned char)(255 * r);
		unsigned char _g = (unsigned char)(255 * g);
		unsigned char _b = (unsigned char)(255 * b);
		unsigned char _a = (unsigned char)(255 * a);
		return (unsigned int)((_a << 24) | (_b << 16) | (_g << 8) | _r);
	}
};