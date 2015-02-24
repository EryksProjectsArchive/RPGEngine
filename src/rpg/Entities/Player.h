/*****************************************************
*
* RPG - 2d game project
* File: Entities/Player.h
* Author: Eryk Dwornicki
*
****************************************************/

#pragma once

#include <ForwardDecls.h>

class Player
{
private:
	unsigned char pad[300];
public:
	Player();
	~Player();

	void Render();
	void Update();
};