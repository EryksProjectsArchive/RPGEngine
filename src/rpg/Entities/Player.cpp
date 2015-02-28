/*****************************************************
*
* RPG - 2d game project
* File: Entities/Player.cpp
* Author: Eryk Dwornicki
*
****************************************************/

#include <Level.h>
#include <Math/Math.h>
#include <Logger.h>

#include "Player.h"

#include <Graphics/Graphics.h>
#include <Graphics/AnimatedSprite.h>
#include <Graphics/Font.h>

#include <Input/Input.h>

Player::Player(Level* level)
{
	m_lastTileType = TILE_TYPE_EMPTY;
	m_level = level;
	m_speed = 200.0f;
	m_isRunning = false;
	m_runPoints = 10.0f;
	m_hudFont = new Font("../Data/Fonts/Rotkiewka.font");

	m_location.scale = Vector2d(0.8f, 0.8f);
	SetPosition(Vector2d(0.0f, 0.0f));
	
	m_sprite = new AnimatedSprite("../Data/Definitions/AnimatedSprites/Player.as");
	m_sprite->PlayAnim("Idle");
}

Player::~Player()
{
	if (m_sprite)
	{
		delete m_sprite;
		m_sprite = 0;
	}

	if (m_hudFont)
	{
		delete m_hudFont;
		m_hudFont = 0;
	}
}

void Player::MoveForward(float steps)
{
	float rot = GetRotation();
	m_velocity -= Vector2d(-sinf(rot), cosf(rot)) * m_speed * steps;
	
	LevelBounds bounds = m_level->GetBounds();
	if (m_location.position.x < bounds.min.x)
		m_location.position.x = bounds.min.x;

	if (m_location.position.x > bounds.max.x)
		m_location.position.x = bounds.max.x;

	if (m_location.position.y < bounds.min.y)
		m_location.position.y = bounds.min.y;

	if (m_location.position.y > bounds.max.y)
		m_location.position.y = bounds.max.y;
}

void Player::MoveBackward(float steps)
{
	float rot = GetRotation();
	m_velocity += Vector2d(-sinf(rot), cosf(rot)) * m_speed * steps;

	LevelBounds bounds = m_level->GetBounds();
	if (m_location.position.x < bounds.min.x)
		m_location.position.x = bounds.min.x;

	if (m_location.position.x > bounds.max.x)
		m_location.position.x = bounds.max.x;

	if (m_location.position.y < bounds.min.y)
		m_location.position.y = bounds.min.y;

	if (m_location.position.y > bounds.max.y)
		m_location.position.y = bounds.max.y;
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

Vector2d Player::GetPosition()
{
	return m_location.position;
}

void Player::SetRotation(float rotation)
{
	m_location.rotation = rotation;
	UpdateMatrix();
}

float Player::GetRotation()
{
	return m_location.rotation;
}

float r = 0.0f;
void Player::Render(Graphics& graphics)
{	
	graphics.DrawSprite(m_sprite, m_matrix);

	Vector2d npc(100, 100);
	Vector2d pos = m_location.position;
	Vector2d dir = (pos - npc);

	if (dir.Length() < 500.0f)
	{
		Vector2d dirn = pos.Normalized();
		r = atan2f(dirn.x, -dirn.y);
	}
	graphics.DrawSprite(m_sprite, glm::translate(glm::mat4(), glm::vec3(100, 100, 0)) * glm::mat4(glm::rotate(glm::mat3(), r)));

	// Draw hud
	m_hudFont->Draw(&graphics, Vector2d(3, 24), Color(1, 0, 0, 0), "Run points (%d):", (int)ceil(m_runPoints));
	m_hudFont->Draw(&graphics, Vector2d(2, 23), Color(1, 1, 1, 1), "Run points (%d):", (int)ceil(m_runPoints));
	graphics.DrawRectangle(Rectangle(1, 40, 200, 20), Color(0.5f, 0, 0, 0));
	graphics.DrawRectangle(Rectangle(3, 42, 196, 16), Color(0.5f, 1, 0.4f, 0));
	graphics.DrawRectangle(Rectangle(3, 42, 196 * (m_runPoints / 10.0f), 16), Color(1.0f, 1, 0.4f, 0));
	// sdsd
}

void Player::Update(float deltaTime)
{
	m_sprite->Update(deltaTime);
	m_isRunning = Input::IsKeyDown(GLFW_KEY_LEFT_SHIFT);
	m_speed = (m_isRunning ? 500.0f : 200.0f);

	if (m_isRunning)
	{
		m_runPoints -= 2.0f * deltaTime;	
		if (m_runPoints < 0.0f)
		{
			m_isRunning = false;
			m_speed = 200.0f;
			m_runPoints = 0.0f;
		}
	}
	else 
	{
		if (m_runPoints < 10.0f)
		{
			m_runPoints += 0.2f * deltaTime;
			if (m_runPoints > 10.0f)			
				m_runPoints = 10.0f;			
		}
	}

	if (Input::IsKeyDown(GLFW_KEY_LEFT))
	{
		float rot = Math::RadianToDegress(GetRotation());
		
		rot -= (100.0f * deltaTime);
		if (rot <= 0.0f)
			rot = 360.0f;

		SetRotation(Math::DegressToRadian(rot));
	}
	else if(Input::IsKeyDown(GLFW_KEY_RIGHT))
	{
		float rot = Math::RadianToDegress(GetRotation());

		rot += (100.0f * deltaTime);
		if (rot >= 360.0f)
			rot = 0.0f;

		SetRotation(Math::DegressToRadian(rot));
	}
		
	if (Input::IsKeyDown(GLFW_KEY_UP))
	{
		MoveForward(1.0f);
	}
	else if (Input::IsKeyDown(GLFW_KEY_DOWN))
	{
		MoveBackward(1.0f);
	}

	TileType type = m_level->GetTypeAt(GetPosition());
	switch (type)
	{
	case TILE_TYPE_ICE:
		MoveForward(1.0f);
		break;
	case TILE_TYPE_DIRT:
		m_velocity *= 0.5;
		break;
	}
	m_lastTileType = type;
	
	float velLength = m_velocity.Length();
	if (velLength > 0.0f)
	{
		m_location.position += m_velocity * deltaTime;
		UpdateMatrix();
		m_velocity /= 10 * deltaTime;

		const char * desiredAnimation = m_isRunning ? "Run" : "Walk";
		if (strcmp(m_sprite->GetCurrentAnimName(), desiredAnimation) != 0)
			m_sprite->PlayAnim(desiredAnimation);			
	}
	else
	{
		if (strcmp(m_sprite->GetCurrentAnimName(), "Idle") != 0)		
			m_sprite->PlayAnim("Idle");		
	}
} 