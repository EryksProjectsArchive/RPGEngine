/*****************************************************
*
* RPG - 2d game project
* File: Math/Rectangle.h
* Author: Eryk Dwornicki
*
*****************************************************/
#pragma once

#include <ForwardDecls.h>
#include "Vector2d.h"

class Rectangle
{
public:
	float x;
	float y;
	float width;
	float height;

	Rectangle()
	{
		x = y = width = height = 0.0f;
	}

	Rectangle(float _x, float _y, float _width, float _height)
	{
		x = _x;
		y = _y;
		width = _width;
		height = _height;
	}

	Rectangle(const Rectangle& rect)
	{
		x = rect.x;
		y = rect.y;
		width = rect.width;
		height = rect.height;
	}

	~Rectangle()	
	{
	}

	float Top() const
	{
		return y;
	}

	float Bottom() const
	{
		return y + height;
	}

	float Left() const
	{
		return x;
	}

	float Right() const
	{
		return x + width;
	}

	bool IsEmpty()
	{
		return (x == 0.0f && y == 0.0f && width == 0.0f && height == 0.0f);
	}

	bool Contains(float x, float y)
	{
		return (x >= Left() && x <= Right() && y >= Top() && y <= Bottom());
	}

	bool Contains(const Vector2d& vector)
	{
		return Contains(vector.x, vector.y);
	}
	
	bool Contains(const Vector2d& vector, const float radius)
	{
		Vector2d vec = vector;
		return Contains(vec + Vector2d(0, radius)) 
			&& Contains(vec - Vector2d(0, radius))
			&& Contains(vec + Vector2d(radius, 0)) 
			&& Contains(vec - Vector2d(radius, 0))
			&& Contains(vec + Vector2d(radius, radius)) 
			&& Contains(vec - Vector2d(radius, radius));
	}

	// full boolean indicates if we are checking if we fully contain this rectangle
	bool Contains(const Rectangle& rect, bool full = true)
	{
		if (full)
			return rect.Left() >= Left() && rect.Right() <= Right() && rect.Top() >= Top() && rect.Bottom() <= Bottom();

		
		return !(rect.Left() <= Left() && rect.Right() >= Right() && rect.Top() <= Top() && rect.Bottom() >= Bottom());
	}

	Rectangle& operator=(const Rectangle& rect)
	{
		x = rect.x;
		y = rect.y;
		width = rect.width;
		height = rect.height;
		return *this;
	}

	bool operator==(const Rectangle& rect)
	{
		return (x == rect.x) && (y == rect.y) 
			&& (width == rect.width) && (height == rect.height);
	}

	bool operator!=(const Rectangle& rect)
	{
		return (x != rect.x) || (y != rect.y) 
			|| (width != rect.width) || (height != rect.height);
	}
		
	static Rectangle Empty()
	{
		return Rectangle();
	}
};