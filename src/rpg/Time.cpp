/*****************************************************
*
* RPG - 2d game project
* File: Time.cpp
* Author: Eryk Dwornicki
*
*****************************************************/

#include "Time.h"

#include <Windows.h>

unsigned long long Time::ms_startTime = 0;
double Time::ms_frequency = 0.0;

void Time::Initialize()
{
	LARGE_INTEGER li;
	if (QueryPerformanceFrequency(&li))	
		ms_frequency = static_cast<double>(li.QuadPart) / 1000000.0;
	
	LARGE_INTEGER lint;
	QueryPerformanceCounter(&lint);
	ms_startTime = lint.QuadPart;
}

unsigned long long Time::Get()
{
	LARGE_INTEGER lint;
	QueryPerformanceCounter(&lint);
	return static_cast<unsigned long long>((lint.QuadPart - ms_startTime) / ms_frequency);
}
