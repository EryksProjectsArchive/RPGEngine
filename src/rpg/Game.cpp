/*****************************************************
*
* RPG - 2d game project
* File: Game.cpp
* Author: Eryk Dwornicki
*
****************************************************/

#include "Game.h"
#include "Memory.h"

#include "Entities/Player.h"

Game::Game()
{
	m_player = new Player();
}

Game::~Game()
{
	if (m_player)
	{
		delete m_player;
		m_player = NULL;
	}
}

bool Game::OnTick()
{
	if (m_player)
		m_player->Update();

	return true;
}