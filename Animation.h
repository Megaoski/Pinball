#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#define MAX_FRAMES 35

class Animation
{
public:
	bool loop = true;
	float speed = 1.0f;
	SDL_Rect frames[MAX_FRAMES];

private:
	float current_frame = 0.0f;
	int last_frame = 0;
	int loops = 0;

public:

	Animation()
	{}

	Animation(const Animation& anim) : loop(anim.loop), speed(anim.speed), last_frame(anim.last_frame)
	{
		SDL_memcpy(&frames, anim.frames, sizeof(frames));
	}

	void PushBack(const SDL_Rect& rect)
	{
		frames[last_frame++] = rect;
	}

	SDL_Rect& GetCurrentFrame()
	{
		current_frame += speed;
		if (current_frame >= last_frame && loop)
		{
			current_frame = 0;
			loops++;
		}
		else if (current_frame >= last_frame && !loop)
		{
			current_frame = last_frame;
			loops++;
		}

		return frames[(int)current_frame];
	}

	SDL_Rect& GetActualFrame()
	{
		return frames[(int)current_frame];
	}

	bool Finished() const
	{
		return loops > 0;
	}

	void setframe(unsigned int frame)
	{
		current_frame = (float)frame;
	}

	void Reset()
	{
		current_frame = 0.0f;
		loops = 0;
	}
};

#endif