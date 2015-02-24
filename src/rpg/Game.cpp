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
	m_player = Memory::Allocate<Player>(1, true);
}

Game::~Game()
{
	Memory::Release(&m_player);
}

bool Game::OnTick()
{
	if (m_player)
		m_player->Update();

	return true;
}