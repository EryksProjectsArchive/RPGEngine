/*****************************************************
*
* RPG - 2d game project
* File: Entities/Player.h
* Author: Eryk Dwornicki
*
****************************************************/

#pragma once

#include <ForwardDecls.h>
#include <Memory.h>
#include <Math/Location.h>

class Player
{
private:
	Location m_location;
	glm::mat4 m_matrix;

	void UpdateMatrix();
public:
	Player();
	~Player();

	void SetPosition(const Vector2d& position);
	Vector2d Position();

	void SetRotation(float rotation);
	float Rotation();

	void Render(Graphics& graphics);
	void Update();
};