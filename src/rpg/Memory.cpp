/*****************************************************
*
* RPG - 2d game project
* File: Memory.cpp
* Author: Eryk Dwornicki
*
*****************************************************/

#include "Memory.h"

size_t Memory::ms_memoryLimit = 0; // 500 mb
size_t Memory::ms_allocatedMemory = 0;

bool Memory::SetMemoryLimit(size_t memoryLimit)
{
	if (ms_allocatedMemory > memoryLimit)
	{
		Warning("[memory] Unable to change limit. New pool is too small");
		return false;
	}

	ms_memoryLimit = memoryLimit;
	Info("[memory] New pool size: %f MB", (float)memoryLimit / 1024 / 1024);
	return true;
}

size_t Memory::GetMemoryLimit()
{
	return ms_memoryLimit;
}