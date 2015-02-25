/*****************************************************
*
* RPG - 2d game project
* File: Vector2d
* Author: Eryk Dwornicki
*
*****************************************************/

#pragma once

#include <Math/Math.h>
#include <Math/Vector2d.h>

class Location
{
public:
	Vector2d position;
	float rotation;
	Vector2d scale;

	Location()
	{
		rotation = 0.0f;
		scale = Vector2d(2.0f, 2.0f);
	}
	
	Location(const Location& loc)
	{
		position = loc.position;
		rotation = loc.rotation;
		scale = loc.scale;
	}

	~Location()
	{
	}

	glm::mat4 ToMatrix()
	{
		return glm::translate(glm::mat4(), glm::vec3(position.x, position.y, 0)) * glm::scale(glm::mat4(), glm::vec3(scale.x, scale.y, 1)) * glm::mat4(glm::rotate(glm::mat3(), rotation));
	}
};