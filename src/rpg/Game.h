/*****************************************************
*
* RPG - 2d game project
* File: Game.h
* Author: Eryk Dwornicki
*
*****************************************************/

#pragma once

#include <ForwardDecls.h>

class Game
{
private:
	Player * m_player;
	Graphics * m_graphics;
	Level * m_level;
	
	bool m_running;
	
	unsigned long long m_lastTick;
public:
	Game();
	~Game();

	bool OnTick();
};