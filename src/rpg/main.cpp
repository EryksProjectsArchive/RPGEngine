/*****************************************************
 * 
 * RPG - 2d game project
 * File: main.cpp
 * Author: Eryk Dwornicki
 *
 ****************************************************/

#include "Game.h"

int main()
{
	Game *game = new Game();
	while (game->OnTick());
	delete game;
	return 1;
}