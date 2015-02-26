/*****************************************************
*
* RPG - 2d game project
* File: Entities/Player.h
* Author: Eryk Dwornicki
*
****************************************************/

#pragma once

#include <ForwardDecls.h>
#include <Math/Location.h>

class Player
{
private:
	Vector2d m_velocity;
	Location m_location;
	glm::mat4 m_matrix;
	AnimatedSprite* m_sprite;
	float m_speed;
	bool m_isRunning;
	Level *m_level;
	TileType m_lastTileType;

	void UpdateMatrix();	
public:
	Player(Level * level);
	~Player();

	void MoveForward(float steps);
	void MoveBackward(float steps);

	void SetPosition(const Vector2d& position);
	Vector2d GetPosition();

	void SetRotation(float rotation);
	float GetRotation();

	void Render(Graphics& graphics);
	void Update(float deltaTime);
};