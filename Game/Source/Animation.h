#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#define MAX_FRAMES 60

class Animation
{
public:
	float speed = 1.0f;
	SDL_Rect frames[MAX_FRAMES];
	bool loop = true;
	// Allows the animation to keep going back and forth
	bool pingpong = false;
	int totalFrames = 0;

private:
	float currentFrame = 0.0f;
	
	int loopCount = 0;
	int pingpongDirection = 1;

public:

	void PushBack(const SDL_Rect& rect)
	{
		frames[totalFrames++] = rect;
	}

	void Reset()
	{
		currentFrame = 0;
	}
	
	bool HasFinished()
	{
		return !loop && !pingpong && loopCount > 0;
	}

	void Update(float dt)
	{
		currentFrame += speed * dt;
		if (currentFrame >= totalFrames)
		{
			currentFrame = (loop || pingpong) ? 0.0f : totalFrames - 1;
			++loopCount;

			if (pingpong)
				pingpongDirection = -pingpongDirection;
		}
	}

	const SDL_Rect& GetCurrentFrame() const
	{
		int actualFrame = currentFrame;
		if (pingpongDirection == -1)
			actualFrame = totalFrames - currentFrame;

		return frames[actualFrame];
	}
<<<<<<< Updated upstream
=======

	int GetTotalFrames() const
	{
		return totalFrames;
	}

	void GenerateAnimation(const SDL_Rect& rect, int rows, int columns)
	{

		int frameWidth = rect.w / columns;
		int frameHeight = rect.h / rows;

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				SDL_Rect frame;
				frame.x = rect.x + (j * frameWidth);
				frame.y = rect.y + (i * frameHeight);
				frame.w = frameWidth;
				frame.h = frameHeight;
				PushBack(frame);
			}
		}
	}
>>>>>>> Stashed changes
};

#endif
