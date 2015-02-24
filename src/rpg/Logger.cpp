/*****************************************************
*
* RPG - 2d game project
* File: Logger.cpp
* Author: Eryk Dwornicki
*
*****************************************************/

#include "Logger.h"
#include "Time.h"

#include <stdarg.h>

Logger Singleton<Logger>::ms_singleton;

#define PRINT_TO_SYSTEM_CONSOLE 1

Logger::Logger()
{
	m_file = fopen("game.log", "w+");
}

Logger::~Logger()
{
	if (m_file)
	{
		fclose(m_file);
		m_file = 0;
	}
}

void Logger::SetFile(const char *fileName)
{
	if (m_file)
	{
		fclose(m_file);
		m_file = 0;
	}

	m_file = fopen(fileName, "w+");
}

void Logger::Message(const char *message)
{
	fprintf(m_file, "[%f] ", Time::Get()/1000.0);

	fputs(message, m_file);
#ifdef _WIN32
	fputc('\n\r', m_file);
#else
	fputc('\n', m_file);
#endif
	fflush(m_file);

#if PRINT_TO_SYSTEM_CONSOLE == 1
	printf("[%f] ", Time::Get() / 1000.0);
	printf("%s\n", message);
#endif
}

void Logger::MessageF(const char* message, ...)
{
	fprintf(m_file, "[%f] ", Time::Get()/1000.0);

	va_list args;
	va_start(args, message);
	vfprintf(m_file, message, args);
	va_end(args);
#ifdef _WIN32
	fputc('\n\r', m_file);
#else
	fputc('\n', m_file);
#endif
	fflush(m_file);

#if PRINT_TO_SYSTEM_CONSOLE == 1
	printf("[%f] ", Time::Get() / 1000.0);

	va_start(args, message);
	vprintf(message, args);
	va_end(args);
	printf("\n");
#endif
}

void Warning(const char * message, ...)
{
	char buffer[512] = { 0 };
	va_list args;
	va_start(args, message);
	vsprintf(buffer, message, args);
	va_end(args);
	Logger::Get().MessageF("[warning] %s", buffer);
}

void Error(const char * message, ...)
{
	char buffer[512] = { 0 };
	va_list args;
	va_start(args, message);
	vsprintf(buffer, message, args);
	va_end(args);
	Logger::Get().MessageF("[error] %s", buffer);
}

void Info(const char * message, ...)
{
	char buffer[512] = { 0 };
	va_list args;
	va_start(args, message);
	vsprintf(buffer, message, args);
	va_end(args);
	Logger::Get().MessageF("[info] %s", buffer);
}