/*****************************************************
*
* RPG - 2d game project
* File: Vector2d
* Author: Eryk Dwornicki
*
*****************************************************/

#pragma once

#include "Math.h"

class Vector2d
{
public:
	float x;
	float y;

	Vector2d()
	{
		x = y = 0.0f;
	}

	Vector2d(float _x, float _y)
	{
		x = _x;
		y = _y;
	}

	Vector2d(const Vector2d& vec)
	{
		x = vec.x;
		y = vec.y;
	}

	operator glm::vec2()
	{
		return glm::vec2(x, y);
	}

	~Vector2d()
	{
	}

	Vector2d& operator=(const Vector2d& vec)
	{
		x = vec.x;
		y = vec.y;
		return *this;
	}

	// Length of vector (NOTE: May be slow for some extensive operations. If you want to save some time look at Vector2d::SquaredLength())
	float Length() const
	{
		return sqrt(x * x + y * y);
	}

	// Faster length calculation.
	float SquaredLength() const
	{
		return x * x + y * y;
	}

	// Calculate dot product
	float DotProduct(const Vector2d& vec) const
	{
		return x * vec.x + y * vec.y;
	}

	// Normalize vector
	void Normalize()
	{
		float length = 1.0f / Length();
		x *= length;
		y *= length;		
	}

	// Returns normalized vector
	Vector2d Normalized() const
	{
		Vector2d out = *this;
		out.Normalize();
		return out;
	}

	// Linear interpolation.
	Vector2d Lerp(const Vector2d& end, float n) const
	{
		return Vector2d(x + (end.x - x) * n, y + (end.y - y) * n);
	}

	// Negate vector
	Vector2d& operator-()
	{
		x = -x;
		y = -y;
		return *this;
	}

	Vector2d& operator+=(const Vector2d& vec)
	{
		x += vec.x;
		y += vec.y;
		return *this;
	}

	Vector2d& operator-=(const Vector2d& vec)
	{
		x -= vec.x;
		y -= vec.y;
		return *this;
	}

	Vector2d& operator*=(const Vector2d& vec)
	{
		x *= vec.x;
		y *= vec.y;
		return *this;
	}

	Vector2d& operator/=(const Vector2d& vec)
	{
		x /= vec.x;
		y /= vec.y;
		return *this;
	}

	Vector2d& operator+(const Vector2d& vec)
	{
		x += vec.x;
		y += vec.y;
		return *this;
	}

	Vector2d& operator-(const Vector2d& vec)
	{
		x -= vec.x;
		y -= vec.y;
		return *this;
	}

	Vector2d& operator*(const Vector2d& vec)
	{
		x *= vec.x;
		y *= vec.y;
		return *this;
	}

	Vector2d& operator/(const Vector2d& vec)
	{
		x /= vec.x;
		y /= vec.y;
		return *this;
	}

	Vector2d& operator+=(float scalar)
	{
		x += scalar;
		y += scalar;
		return *this;
	}

	Vector2d& operator-=(float scalar)
	{
		x -= scalar;
		y -= scalar;
		return *this;
	}

	Vector2d& operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	Vector2d& operator/=(float scalar)
	{
		x /= scalar;
		y /= scalar;
		return *this;
	}

	Vector2d& operator+(float scalar)
	{
		x += scalar;
		y += scalar;
		return *this;
	}

	Vector2d& operator-(float scalar)
	{
		x -= scalar;
		y -= scalar;
		return *this;
	}

	Vector2d& operator*(float scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	Vector2d& operator/(float scalar)
	{
		x /= scalar;
		y /= scalar;
		return *this;
	}
};