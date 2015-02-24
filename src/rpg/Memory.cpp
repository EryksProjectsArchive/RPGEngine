/*****************************************************
*
* RPG - 2d game project
* File: Memory.cpp
* Author: Eryk Dwornicki
*
*****************************************************/

#include "Memory.h"

size_t Memory::ms_memoryLimit = 1024 * 1024 * 500; // 500 mb
size_t Memory::ms_allocatedMemory = 0;

bool Memory::SetMemoryLimit(size_t memoryLimit)
{
	if (ms_allocatedMemory > memoryLimit)
		return false;

	ms_memoryLimit = memoryLimit;
	return true;
}

size_t Memory::GetMemoryLimit()
{
	return ms_memoryLimit;
}