/*****************************************************
*
* RPG - 2d game project
* File: Game.cpp
* Author: Eryk Dwornicki
*
*****************************************************/

#include <stdio.h>

#include "Game.h"
#include "Level.h"
#include "Time.h"

#include "Entities/Player.h"

#include "Graphics/Graphics.h"
#include "Graphics/AnimatedSprite.h"
#include "Graphics/Font.h"

#include "Input/Input.h"

Game::Game()
{	
	m_graphics = new Graphics();
	m_level = new Level();
	m_player = new Player(m_level);
	m_running = true;
	m_lastTick = 0;
	m_baseFont = new Font("../Data/Fonts/Rotkiewka.font");
}

Game::~Game()
{
	if (m_player)
	{
		delete m_player;
		m_player = NULL;
	}

	if (m_graphics)
	{
		delete m_graphics;
		m_graphics = NULL;
	}

	if (m_level)
	{
		delete m_level;
		m_level = NULL;
	}

	if (m_baseFont)
	{
		delete m_baseFont;
		m_baseFont = 0;
	}
}

bool Game::OnTick()
{
	unsigned long long now = Time::Get();
	float deltaTime = (float)(now - m_lastTick) / 1000.0f / 1000.0f;
	// Update window
	if (m_graphics)
	{
		/*char title[32] = { 0 };
		sprintf(title, "RPG - FPS: %.01f", 1.0/deltaTime);
		m_graphics->SetWindowTitle(title);*/		
		if (!m_graphics->UpdateWindow())
		{
			m_running = false;
		}
	}

	if (Input::IsKeyDown(GLFW_KEY_ESCAPE))
		m_running = false;

	// Update game logic
	if (m_player)
		m_player->Update(deltaTime);

	// Render
	if (m_graphics)
	{
		m_graphics->PreRender();

		m_level->Render(*m_graphics);

		m_player->Render(*m_graphics);

		float scroll = Input::GetMouseScroll();
		if (scroll > 0.0f)
		{
			float zoom = m_graphics->GetCameraZoom() - (10.0f * deltaTime);
			if (zoom < 1.0f)
				zoom = 1.0f;
			m_graphics->SetCameraZoom(zoom);
		}
		else if (scroll < 0.0f)
		{
			float zoom = m_graphics->GetCameraZoom() + (10.0f * deltaTime);
			if (zoom > 1.5f)
				zoom = 1.5f;
			m_graphics->SetCameraZoom(zoom);
		}

		m_graphics->SetCameraPosition(m_player->GetPosition());

		m_baseFont->Draw(m_graphics, Vector2d(2, 2), Color(1, 0, 0, 0), "FPS: %.01f", 1 / deltaTime);
		m_baseFont->Draw(m_graphics, Vector2d(1, 1), Color(1, 1, 1, 1), "FPS: %.01f", 1 / deltaTime);

		m_graphics->PostRender();
	}

	m_lastTick = now;
	return m_running;
}