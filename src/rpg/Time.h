/*****************************************************
*
* RPG - 2d game project
* File: Time.h
* Author: Eryk Dwornicki
*
*****************************************************/

#pragma once

class Time
{
private:
	static unsigned long long ms_startTime;
	static double ms_frequency;

public:
	static void Initialize();
	static unsigned long long Get();
};