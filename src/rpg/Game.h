/*****************************************************
*
* RPG - 2d game project
* File: Game.h
* Author: Eryk Dwornicki
*
*****************************************************/

#pragma once

#include <ForwardDecls.h>
#include <Memory.h>

class Game
{
private:
	Player * m_player;
	Graphics * m_graphics;
	
	bool m_running;
public:
	Game();
	~Game();

	bool OnTick();
};