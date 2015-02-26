/*****************************************************
*
* RPG - 2d game project
* File: Graphics/AnimatedSprite.cpp
* Author: Eryk Dwornicki
*
*****************************************************/

#include "AnimatedSprite.h"

#include <stdio.h>
#include <cctype>
#include <Logger.h>

namespace Parser
{
	static unsigned int line = 0;
	static unsigned int column = 0;

	bool IsNewLine(char *p)
	{
		return (*p == '\n' || *p == '\r');
	}

	void IncLine()
	{
		line++;
		column = 0;
	}

	void IncColumn()
	{
		column++;
	}

	char * SkipLine(char *p)
	{
		if (*p == '\0')
			return NULL;

		*p++;
		IncColumn();
		if (*p == '\n' || *p == '\r')
		{
			*p++;
			IncColumn();
			if (*p == '\n' || *p == '\r')
			{
				*p++;
				IncColumn();
			}

			IncLine();
			return p;
		}
		return SkipLine(p);
	}

	bool IsAlphaChar(char c)
	{
		return isalpha(c) ? true : false;
	}

	bool IsSpecialChar(char c)
	{
		return c == '\t' || c == '\n' || c == '\r';
	}

	char * SkipSpecials(char *p)
	{
		if (!*p)
			return NULL;

		if (IsSpecialChar(*p))
		{
			if (IsNewLine(p))
			{
				IncLine();
			}
			
			*p++; IncColumn();
			return SkipSpecials(p);
		}			
		return p;
	}

	char * SkipTrash(char *p)
	{
		return SkipSpecials(p);
	}

	char * MoveToNextAlphaString(char *p)
	{
		if (!*p)
			return NULL;

		if (IsAlphaChar(*p))
			return p;

		*p++;
		IncColumn();
		if (IsNewLine(p))		
			p = SkipLine(p);

		return MoveToNextAlphaString(p);
	}

	char * MoveToNextAlphaNumericString(char *p)
	{
		if (!*p)
			return NULL;

		if (isalnum(*p))
			return p;

		*p++;
		IncColumn();
		if (IsNewLine(p))
			p = SkipLine(p);

		return MoveToNextAlphaNumericString(p);
	}
	
	char * ParseValue(char *p, char* name, char* value)
	{
		if (*p == '\0') 
			return NULL;

		MoveToNextAlphaString(p);		
		SkipSpecials(p);
		
		if (*p == '\0') 
			return NULL;

		// Parse name
		unsigned int i = 0;
		while (IsAlphaChar(*p))
		{
			*name++ = *p++; IncColumn();

			i++;
			if (i >= 32)
			{
				Error("[Parser] Name of value is too long max 32 characters! Line: %d Column: %d", line, column);
				return NULL;
			}
		}
		*name = '\0';

		p = MoveToNextAlphaNumericString(p);
		if (!p)
			return NULL;

		// Parse value
		i = 0;
		while (!IsSpecialChar(*p) && *p)
		{
			*value++ = *p++; IncColumn();

			i++;
			if (i >= 32)
			{
				Error("[Parser] Value is too long max 32 characters! Line: %d Column: %d", line, column);
				return NULL;
			}
		}		
		*value = '\0';
		return p;
	}

	bool IsSection_(char *str)
	{
		if (!*str || (!IsAlphaChar(*str) && *str != ':'))
			return false;

		if (*str == ':')
			return true;

		*str++;
		return IsSection_(str);
	}

	bool IsSection(const char *str)
	{
		size_t size = strlen(str);
		char *p = new char[size + 1];
		strcpy(p, str);
		p[size] = '\0';
		bool section = IsSection_(p);
		delete[]p;
		return section;
	}

	char * ParseSection(char *p, char *name)
	{
		if (!*p)
			return NULL;

		p = SkipTrash(p);

		unsigned int i = 0;
		while (*p && IsAlphaChar(*p) && *p != ':' && i < 32)
		{
			*name++ = *p++; IncColumn();
			i++;
		}
		if (i >= 32)
		{
			Error("[Parser] Section name is too long max 32 characters! Line: %d Column: %d", line, column);
			return NULL;
		}
		return p;
	}
	
	enum Types
	{
		SECTION,
		UNSUPPORTED
	};
};

#include <list>

struct DefinitionValue
{
	char name[32];
	char value[32];
};

struct DefinitionSection
{
	char name[32];
	std::list<DefinitionValue *> values;
};

AnimatedSprite::AnimatedSprite(const char* fileName)
	: m_animations(NULL), m_currentlyPlayedAnim(NULL), m_animationsCount(0), m_playing(false), m_currentFrame(0), m_timeToChangeFrame(0.0f)
{
	Parser::line = 0;
	Parser::column = 0;

	FILE *fp = fopen(fileName, "rb");
	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		long size = ftell(fp);
		rewind(fp);

		char *buffer = new char[size+1];
		memset(buffer, 0, size+1);
		fread(buffer, sizeof(char), size, fp);
		buffer[size] = '\0';

		char *p = buffer;

		std::list<DefinitionSection *> sections;
		DefinitionSection *sec = NULL;
		Parser::Types current = Parser::UNSUPPORTED;
		while (*p)
		{
			// Skip comments
			if (*p == '#')
			{
				p = Parser::SkipLine(p);
				if (!p) 
					break;
			}

			p = Parser::SkipSpecials(p);
			if (!p)
				break;

			char name[32] = { 0 };	
			if (Parser::IsSection(p))
			{				
				if (!(p = Parser::ParseSection(p, name)))
					break;

				Info("New section %s", name);

				DefinitionSection * section = new DefinitionSection;
				sec = section;
				strcpy(section->name, name);	
				sections.push_back(section);

				p = Parser::SkipLine(p);
				if (!p)
					break;

				current = Parser::SECTION;
				continue;
			}
			
			if (current == Parser::SECTION)
			{
				char name[32] = { 0 };
				char value[32] = { 0 };
				p = Parser::ParseValue(p, name, value);
				if (!p) 
					break;
				
				Info("[as] %s %s", name, value);

				if (sec)
				{
					DefinitionValue *val = new DefinitionValue;
					strcpy(val->name, name);
					strcpy(val->value, value);
					sec->values.push_back(val);
				}
			}
			
			if (!p || (*p == 0))
				break;
			
			*p++;
			Parser::IncColumn();
		}

		for (DefinitionSection * sectionRef : sections)
		{
			Info("SEC: %s", sectionRef->name);

			for (DefinitionValue * secValueRef : sectionRef->values)
			{
				Info("> Name: %s Value: %s", secValueRef->name, secValueRef->value);
			}
		}

		delete[]buffer;

		fclose(fp);
	}
}

AnimatedSprite::~AnimatedSprite()
{
	m_currentlyPlayedAnim = NULL;
	if (m_animations)
	{
		delete[]m_animations;
		m_animations = NULL;
	}
}

bool AnimatedSprite::IsCurrentlyPlayedAnim(const char* name)
{
	if (m_currentlyPlayedAnim)
	{
		return !strcmp(m_currentlyPlayedAnim->name, name);
	}
	return false;
}

bool AnimatedSprite::PlayAnim(const char *name)
{
	for (unsigned int i = 0; i < m_animationsCount; ++i)
	{
		if (!strcmp(m_animations[i].name, name))
		{
			m_currentlyPlayedAnim = &m_animations[i];
			m_timeToChangeFrame = m_currentlyPlayedAnim->interval;
			m_playing = true;
			return true;
			break;
		}
	}
	m_playing = false;
	return false;	
}

void AnimatedSprite::PauseAnim()
{
	m_currentlyPlayedAnim = NULL;
	m_playing = false;
}

const char * AnimatedSprite::GetCurrentAnimName()
{
	if (m_currentlyPlayedAnim)
		return m_currentlyPlayedAnim->name;

	return "None";
}

AnimationFrame AnimatedSprite::GetCurrentFrameData()
{
	return m_currentFrameData;
}

void AnimatedSprite::Update(float deltaTime)
{
	if (m_playing && m_currentlyPlayedAnim)
	{
		if (m_currentlyPlayedAnim->framesCount > 1)
		{
			m_timeToChangeFrame -= deltaTime;
			if (m_timeToChangeFrame <= 0.0f)
			{
				m_currentFrame++;
				if (m_currentFrame > m_currentlyPlayedAnim->framesCount)//LOOP (TODO: Break on end?)
					m_currentFrame = 0;

				m_currentFrameData = m_currentlyPlayedAnim->frames[m_currentFrame];
				m_timeToChangeFrame = m_currentlyPlayedAnim->interval;
			}
		}
	}
}