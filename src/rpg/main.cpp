/*****************************************************
 * 
 * RPG - 2d game project
 * File: main.cpp
 * Author: Eryk Dwornicki
 *
 ****************************************************/

#include "Time.h"
#include "Logger.h"

#include "Game.h"

int main()
{
	Time::Initialize();
	Logger::Get().SetFile("../game.log");

	Game *game = new Game();
	while (game->OnTick());
	delete game;
	return 1;
}