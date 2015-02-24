/*****************************************************
*
* RPG - 2d game project
* File: Memory.h
* Author: Eryk Dwornicki
*
*****************************************************/

#pragma once

#include <Windows.h>

#include <malloc.h>
#include <new>
#include <stdio.h>

#ifndef NULL
#define NULL 0
#endif

// Basic allocator
class Memory
{
private:
	static size_t ms_memoryLimit;
	static size_t ms_allocatedMemory;
public:
	static bool SetMemoryLimit(size_t memoryLimit);
	static size_t GetMemoryLimit();

	// This function allocates memory
	template <typename T>
	static T* Allocate(size_t count = 1, bool initialize = false)
	{
		if (count < 1) 
			return NULL;

		size_t bytes = sizeof(T) * count;
		if ((ms_allocatedMemory + bytes) >= ms_memoryLimit)
		{
			printf("[memory] Memory limit reached (%f MB/%f MB)\n", ((float)ms_allocatedMemory / 1024 / 1024), ((float)ms_memoryLimit / 1024 / 1024));
			RaiseException(EXCEPTION_ACCESS_VIOLATION, 0, 0, NULL);
			return NULL;
		}
		
		T* mem = (T *)malloc(bytes);
		if (mem)
		{
			ms_allocatedMemory += bytes;
			if (initialize)
			{
				if (count > 1)
					mem = new (mem)T[count];
				else
					mem = new (mem)T();
			}
		}
		return mem;
	}

	// This function is safely removing element from memory
	template <typename T>
	static bool Release(T ** ptr)
	{
		if (*ptr)
		{
			ms_allocatedMemory -= sizeof(*ptr);
			free(*ptr);
			*ptr = NULL;
			return true;
		}
		return false;
	}
};