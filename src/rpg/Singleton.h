/*****************************************************
*
* RPG - 2d game project
* File: Singleton.h
* Author: Eryk Dwornicki
*
*****************************************************/
#pragma once

template <typename T>
class Singleton
{
private:
	static T ms_singleton;

public:
	inline static T& Get() { return ms_singleton; };
	inline static T* GetPtr() { return &ms_singleton; }
};