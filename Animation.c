//------------------------------------------------------------------------------
//
// File Name:	Animation.c
// Author(s):	Joe Centeno(joe.centeno)
// Project:		Project 2
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "Animation.h"
#include "Trace.h"
#include "Stream.h"
#include "Sprite.h"
#include "SpriteSource.h"
#include "GameObject.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct Animation
{
	// Pointer to the parent game object associated with the animation.
	GameObjectPtr parent;

	// The current frame being displayed.
	unsigned int frameIndex;

	// The maximum number of frames in the sequence.
	unsigned int frameCount;

	// The time remaining for the current frame.
	float frameDelay;

	// The amount of time to display each successive frame.
	float frameDuration;

	// True if the animation is running; false if the animation has stopped.
	bool isRunning;

	// True if the animation loops back to the beginning.
	bool isLooping;

	// True if the end of the animation has been reached, false otherwise.
	// (Hint: This should be true for only one game loop.)
	bool isDone;

	// The animation sequence currently being played, if any.
	//AnimationSequencePtr	sequence;

} Animation;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

void AnimationAdvanceFrame(AnimationPtr animation);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

AnimationPtr AnimationCreate(void)
{
	Animation* animPtr = calloc(1, sizeof(Animation));

	if (animPtr)
	{
		animPtr->frameCount = 0;
		animPtr->frameDelay = 0.0f;
		animPtr->frameDuration = 0.0f;
		animPtr->frameIndex = 0;
		animPtr->isDone = false;
		animPtr->isLooping = false;
		animPtr->isRunning = false;
		return animPtr;
	}
	TraceMessage("Error: AnimationCreate could not create an animation ptr");
	return NULL;
}

AnimationPtr AnimationClone(const AnimationPtr other)
{
	if (other)
	{
		AnimationPtr NewAnimation = AnimationCreate();
		*NewAnimation = *other;
		return NewAnimation;
	}
	else if(other == NULL)
	{
		//TraceMessage("Error: Animation Clone needs a vaild pointer");
	}

	return NULL;
}

void AnimationFree(AnimationPtr* animation)
{
	if (!animation)
		return;
	free(*animation);
	*animation = NULL;
	return;
}

void AnimationRead(AnimationPtr animation, Stream stream)
{
	if (animation && stream)
	{
		animation->frameIndex = StreamReadInt(stream);
		animation->frameCount = StreamReadInt(stream);

		animation->frameDelay = StreamReadFloat(stream);
		animation->frameDuration = StreamReadFloat(stream);

		animation->isRunning = StreamReadBoolean(stream);
		animation->isLooping = StreamReadBoolean(stream);
		return;
	}
	TraceMessage("Error: AnimationRead requires a valid Animation ptr and a valid Stream");
	return;
}

void AnimationSetParent(AnimationPtr animation, GameObjectPtr parent)
{
	if (animation && parent)
	{
		animation->parent = parent;
		return;
	}
	TraceMessage("Error: AnimationSetParent requires a valid Animation ptr and a valid GameObject ptr");
	return;
}

void AnimationPlay(AnimationPtr animation, int frameCount, float frameDuration, bool isLooping)
{
	if (animation)
	{
		animation->isDone = false;
		animation->isRunning = true;
		animation->frameIndex = 0;
		animation->frameCount = frameCount;
		animation->frameDuration = frameDuration;
		animation->frameDelay = frameDuration;
		animation->isLooping = isLooping;
		
		SpriteSetFrame(GameObjectGetSprite(animation->parent), animation->frameIndex);
		return;
	}
	TraceMessage("Error: AnimationPlay requires a valid Animation ptr");
	return;
}

void AnimationUpdate(AnimationPtr animation, float dt)
{
	if (animation)
	{
		animation->isDone = false;
		if (animation->isRunning)
		{
			animation->frameDelay -= dt;
			if (animation->frameDelay <= 0)
			{	
				AnimationAdvanceFrame(animation);
			}
		}
		return;
	}
	TraceMessage("Error: AnimationUpdate requires a valid Animation ptr");
	return;
}

void AnimationAdvanceFrame(AnimationPtr animation)
{
	if (!animation) {
		TraceMessage("Error: AnimationAdvanceFrame requires a valid Animation ptr");
		return;
	}

	animation->frameIndex++;
	if (animation->frameIndex >= animation->frameCount)
	{
		animation->isDone = true;

		if (animation->isLooping)
		{
			animation->frameIndex = 0;
		} else {
			animation->frameIndex = animation->frameCount - 1;
			animation->isRunning = false;
		}
	}
	if (animation->isRunning)
	{
		SpriteSetFrame(GameObjectGetSprite(animation->parent), animation->frameIndex);
		animation->frameDelay += animation->frameDuration;
	} else
		animation->frameDelay = 0;
}

bool AnimationIsDone(AnimationPtr animation)
{
	if (animation)
	{
		return animation->isDone;
	}
		
	TraceMessage("Error: AnimationIsDone requires a valid Animation ptr");
	return false;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
