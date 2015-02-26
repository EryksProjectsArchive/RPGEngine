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
		if (p)		
			return *p == '\n' || *p == '\r';
		
		return false;
	}

	bool IsEndOfString(char *p)
	{
		if (p)		
			return *p == '\0';
		
		return false;
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
		return isalpha(c) != 0;
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

	char * GoToSeparatorAndSkipIt(char *p, char separator)
	{
		if (!*p || IsNewLine(p) || IsEndOfString(p))
			return NULL;
		
		char c = *p++;		
		return (c == separator) ? p : GoToSeparatorAndSkipIt(p, separator);
	}

	bool ParseListOfUnsignedInt(char **p, char separator, unsigned int * number)
	{
		if (!*p)
			return false;

		char *ptr = *p;
		while (*ptr)
		{
			if (*ptr >= '0' && *ptr <= '9')
			{
				if (sscanf(ptr, "%u", number) == 1)
				{
					*p = GoToSeparatorAndSkipIt(ptr, separator);
					return true;
				}
			}
			*ptr++;
			*p = ptr;
		}
		*p = NULL;
		return false;
	}

	bool ParseFloatFromString(char *p, float * value)
	{
		return sscanf(p, "%f", value) == 1;
	}

	bool ParseUnsignedIntFromString(char *p, unsigned int * value)
	{
		return sscanf(p, "%u", value) == 1;
	}
	
	enum Types
	{
		SECTION,
		UNSUPPORTED
	};
};

#include <list>
#include <vector>

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
	: m_animations(NULL), m_currentlyPlayedAnim(NULL), m_animationsCount(0), m_playing(false), m_currentFrame(0), m_timeToChangeFrame(0.0f), m_texture(0)
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
		
		std::list<DefinitionSection *> tmpAnims;
		DefinitionSection *info = NULL;
		for (DefinitionSection * sectionRef : sections)
		{
			if (!strcmp(sectionRef->name, "Info"))
			{
				info = sectionRef;
			}
			else if(!strcmp(sectionRef->name, "Animation"))
			{
				tmpAnims.push_back(sectionRef);
			}
		}

		unsigned int framesCount = 0;
		unsigned int frameWidth = 0, frameHeight = 0;
		if (info)
		{
			for (DefinitionValue * valueRef : info->values)
			{
				if (!strcmp(valueRef->name, "Texture"))
				{
					if (!m_texture)
					{
						char path[256] = { 0 };
						sprintf(path, "../Data/Sprites/%s", valueRef->value);
						m_texture = new Sprite(path);
					}
					else
					{
						Error("[animation sprite] Duplicate of Into.Texture value in file '%s'.", fileName);
					}
				}
				else if (!strcmp(valueRef->name, "FramesCount"))
				{
					Parser::ParseUnsignedIntFromString(valueRef->value, &framesCount);
				}
				else if (!strcmp(valueRef->name, "FrameWidth"))
				{
					Parser::ParseUnsignedIntFromString(valueRef->value, &frameWidth);
				}
				else if (!strcmp(valueRef->name, "FrameHeight"))
				{
					Parser::ParseUnsignedIntFromString(valueRef->value, &frameHeight);
				}
			}

			// Parse animations
			if (tmpAnims.size() > 0 && m_texture && framesCount > 0 && frameWidth > 0 && frameHeight > 0)
			{
				m_size = Vector2d(frameWidth, frameHeight);
				Vector2d textureSize = m_texture->GetSize();

				std::vector<AnimationFrame> readyToUseFrames;

				Vector2d pos;
				for (unsigned int i = 0; i < framesCount; ++i)
				{
					AnimationFrame frame;
					frame.x = pos.x / textureSize.x;
					frame.y = 1.0f - (pos.y / textureSize.y);
					frame.w = (pos.x + frameWidth) / textureSize.x;
					frame.h = 1.0f - ((pos.y + frameHeight) / textureSize.y);
					readyToUseFrames.push_back(frame);

					pos.x += frameWidth;
					if (pos.x >= textureSize.x)
					{
						pos.y += frameHeight;
						pos.x = 0.0f;
						if (pos.y >= textureSize.y && i < (framesCount-1))
						{
							Error("[animated sprite] Too much frames (%d)!", i);
							break;
						}
					}
				}

				std::list<Animation> animTmp;
				for (DefinitionSection * sectionRef : tmpAnims)
				{
					bool hasName = false, hasInterval = false, hasFrames = false;

					char name[32] = { 0 };
					char interval[32] = { 0 };
					char frames[32] = { 0 };
					for (DefinitionValue * valueRef : sectionRef->values)
					{
						if (!strcmp(valueRef->name, "Name"))
						{
							strcpy(name, valueRef->value);
							hasName = true;
						}
						else if (!strcmp(valueRef->name, "Interval"))
						{
							strcpy(interval, valueRef->value);
							hasInterval = true;
						}
						else if (!strcmp(valueRef->name, "Frames"))
						{
							strcpy(frames, valueRef->value);
							hasFrames = true;
						}
					}

					if (hasName && hasInterval && hasFrames)
					{
						char *p = frames;
						unsigned int number = 0;
						std::list<unsigned int> frameNumbers;
						while (Parser::ParseListOfUnsignedInt(&p, ',', &number))
							frameNumbers.push_back(number);

						float intervalValue = 0.0f;
						if (!Parser::ParseFloatFromString(interval, &intervalValue))
						{
							Error("[animated sprite] Invalid type of '%s'.Animation.Interval for %s - it must be float!", fileName, name);
						}

						Animation animation;
						animation.framesCount = frameNumbers.size();
						strcpy(animation.name, name);
						animation.interval = intervalValue;
						animation.frames = new AnimationFrame[animation.framesCount];
						unsigned int frameIndex = 0;
						
						unsigned int size = readyToUseFrames.size()-1;
						for (unsigned int n : frameNumbers)
						{
							if (n > size)
							{
								Error("[animated sprite] Too high frame index used for %s - used: %d max: %d file: '%s' size: %s", animation.name, n, size, fileName, (size+1) == framesCount ? "valid" : "invalid");
								break;
							}

							// Calculate zone
							animation.frames[frameIndex] = readyToUseFrames[n];
							frameIndex++;
						}
						animTmp.push_back(animation);
					}
					else
					{
						Error("[animated sprite] Lack of Name, Interval and Frames attributes in '%s'.%s.", fileName, sectionRef->name);
					}
				}

				m_animationsCount = animTmp.size();
				m_animations = new Animation[m_animationsCount];
				unsigned int index = 0;
				for (Animation anim : animTmp)
				{
					m_animations[index] = anim;
					index++;
				}				
			}
			else
			{
				// Errors/Warnings
				if (!m_texture) Error("[animated sprite] No texture defined for '%s'.", fileName); // No texture				
				if (!framesCount) Error("[animated sprite] You must specify 'FramesCount' parameter in 'Info' section '%s'.", fileName); // No 'FramesCount' value
				if (!frameWidth || !frameHeight) Error("[animated sprite] You must specify 'FrameWidth' and 'FrameHeight' parameter (that is larger than 0!) in 'Info' section '%s'.", fileName); // Invalid/no 'FrameWidth/Height'value
				if (!tmpAnims.size()) Error("[animated sprite] No animations defined for '%s'.", fileName); // No animations
			}
		}
		else
		{
			Error("[animated sprite] No info section in '%s' file found!", fileName);
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

	if (m_texture)
	{
		delete m_texture;
		m_texture = 0;
	}
}

unsigned int AnimatedSprite::GetTextureId()
{
	return m_texture ? m_texture->GetTextureId() : -1;
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
			m_currentFrame = 0;
			m_currentFrameData = m_currentlyPlayedAnim->frames[m_currentFrame];
			m_playing = true;
			return true;
			break;
		}
	}

	Error("[animated sprite] Unable to play '%s' animation!", name);
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
			m_timeToChangeFrame -= (deltaTime / 1000.0f);
			if (m_timeToChangeFrame <= 0.0f)
			{
				m_currentFrame++;
				if (m_currentFrame >= m_currentlyPlayedAnim->framesCount)//LOOP (TODO: Break on end?)
					m_currentFrame = 0;

				m_currentFrameData = m_currentlyPlayedAnim->frames[m_currentFrame];
				m_timeToChangeFrame = m_currentlyPlayedAnim->interval;
			}
		}
	}
}

Vector2d AnimatedSprite::GetSize()
{
	return m_size;
}