/*****************************************************
*
* RPG - 2d game project
* File: Game.cpp
* Author: Eryk Dwornicki
*
*****************************************************/

#include "Game.h"
#include "Memory.h"

#include "Entities/Player.h"

#include "Graphics/Graphics.h"

Game::Game()
{
	m_player = new Player();
	m_graphics = new Graphics();

	m_running = true;
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
}

float _x = 0.0f;
bool out = false;
bool Game::OnTick()
{
	// Update window
	if (m_graphics)
	{
		if (!m_graphics->UpdateWindow())
		{
			m_running = false;
		}
	}

	// Update game logic
	if (m_player)
		m_player->Update();

	// Render
	if (m_graphics)
	{
		m_graphics->PreRender();

		m_player->Render(*m_graphics);

		m_graphics->SetCameraPosition(Vector2d(sinf(_x) * 10.0f, cosf(_x) * 10.0f));
		m_graphics->SetCameraZoom(1.0f +(sinf(_x) / 5));
		/*m_graphics->SetCameraZoom(_x);
		_x += 5.0f;*/

		if (out)
		{
			_x -= 0.05f;

			if (_x <= 0.0f)
			{
				out = false;
				_x = 0.0f;
			}
		}
		else
		{
			_x += 0.05f;

			if (_x >= 3.0f)
			{
				out = true;
				_x = 3.0f;
			}
		}

		printf("[main loop] C: %f, %f ZOOM: %f\n", m_graphics->GetCameraPosition().x, m_graphics->GetCameraPosition().y, m_graphics->GetCameraZoom());

		m_graphics->PostRender();
	}
	return m_running;
}