/*****************************************************
*
* RPG - 2d game project
* File: Graphics/AnimatedSprite.h
* Author: Eryk Dwornicki
*
*****************************************************/

#include <ForwardDecls.h>

#include "Sprite.h"

struct AnimationFrame
{
	float x, y, w, h;

	AnimationFrame()
	{
		x = 0.0f;
		y = 0.0f;
		w = 0.0f;
		h = 0.0f;
	}

	AnimationFrame& operator=(const AnimationFrame& frame)
	{
		x = frame.x;
		y = frame.y;
		w = frame.w;
		h = frame.h;
		return *this;
	}
};

struct Animation
{
	char name[32];
	float interval;
	unsigned int framesCount;
	AnimationFrame* frames;

	Animation()
	{
		memset(name, 0, sizeof(name));
		interval = 0.0f;
		framesCount = 0;
		frames = NULL;
	}
};

class AnimatedSprite : public Sprite
{
private:
	unsigned int m_animationsCount;
	Animation *m_animations;

	Animation *m_currentlyPlayedAnim;
	bool m_playing;
	float m_timeToChangeFrame;
	unsigned int m_currentFrame;
	AnimationFrame m_currentFrameData;
public:
	AnimatedSprite(const char* fileName);
	virtual ~AnimatedSprite();

	virtual bool IsCurrentlyPlayedAnim(const char* name);
	virtual bool PlayAnim(const char *name);
	virtual void PauseAnim();
	virtual const char * GetCurrentAnimName();

	AnimationFrame GetCurrentFrameData();

	virtual void Update(float deltaTime);
};