/*****************************************************
*
* RPG - 2d game project
* File: Entities/Player.cpp
* Author: Eryk Dwornicki
*
****************************************************/

#include <Math/Math.h>
#include <Logger.h>

#include "Player.h"

#include <Graphics/Graphics.h>

Player::Player()
{
	SetPosition(Vector2d(200.0f, 200.0f));
	//SetRotation(Math::DegressToRadian(50.0f));
}

Player::~Player()
{

}

void Player::UpdateMatrix()
{
	// Update our matrix - matrix is used to draw player.
	m_matrix = m_location.ToMatrix();
}

void Player::SetPosition(const Vector2d& position)
{
	m_location.position = position;	
	UpdateMatrix();
}

Vector2d Player::Position()
{
	return m_location.position;
}

void Player::SetRotation(float rotation)
{
	m_location.rotation = rotation;
	UpdateMatrix();
}

float Player::Rotation()
{
	return m_location.rotation;
}

void Player::Render(Graphics& graphics)
{
	graphics.DrawSprite(NULL, m_matrix);
}

float x = 0.0f;
void Player::Update()
{
	/*SetRotation(Math::DegressToRadian(x));
	x += 5.0f;
	if (x >= 360.0f)
		x = 0.0f;*/
}