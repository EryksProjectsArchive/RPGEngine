/*****************************************************
*
* RPG - 2d game project
* File: Math.h
* Author: Eryk Dwornicki
*
*****************************************************/

#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

#define PI 3.14159265358979323846f

namespace Math
{
	static float DegressToRadian(float degress)
	{
		return degress * PI / 180.0f;
	}

	static float RadianToDegress(float radian)
	{
		return radian * 180.0f / PI;
	}
};