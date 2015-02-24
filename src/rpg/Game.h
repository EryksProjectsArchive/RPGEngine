/*****************************************************
*
* RPG - 2d game project
* File: Game.h
* Author: Eryk Dwornicki
*
****************************************************/

#pragma once

#include <ForwardDecls.h>

class Game
{
private:
	Player * m_player;

public:
	Game();
	~Game();

	bool OnTick();
};