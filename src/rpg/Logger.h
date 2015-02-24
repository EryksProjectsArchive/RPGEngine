/*****************************************************
*
* RPG - 2d game project
* File: Logger.h
* Author: Eryk Dwornicki
*
*****************************************************/

#pragma once

#include <ForwardDecls.h>
#include <stdio.h>

#include <Singleton.h>

class Logger : public Singleton<Logger>
{
private:
	FILE *m_file;
public:
	Logger();
	~Logger();

	void SetFile(const char *fileName);
	
	void Message(const char *message);
	void MessageF(const char* message, ...);
};

void Warning(const char * message, ...);
void Error(const char * message, ...);
void Info(const char * message, ...);