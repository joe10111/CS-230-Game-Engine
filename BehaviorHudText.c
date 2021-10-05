//------------------------------------------------------------------------------
//
// File Name:	BehaviorHudText.c
// Author(s):	Joe Centeno (joe.centeno)
// Project:		Project 5
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "BehaviorHudText.h"

#include "stdafx.h"
#include "BehaviorHudText.h"
#include "Behavior.h"
#include "GameObject.h"
#include "Sprite.h"
#include "ScoreSystem.h"
#include "Stream.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
typedef enum HudTextStates
{
	cHudTextInvalid = -1,	// HUD Text has not yet been initialized.
	cHudTextIdle,			// HUD Text will normally remain static.

} HudTextStates;

typedef struct BehaviorHudText
{
	// Inherit the base behavior structure.
	Behavior	base;

	// Add HUD Text-specific behavior variables.

	// The index used to access values from the Score System.
	ScoreSystemId scoreSystemId;

	// The format string to be used with sprintf_s() when updating the HUD Text object.
	// (For example: "Score: %d")
	// (NOTE: This buffer has an arbitrary length of 32 chars, which is sufficient for this project.
	//	Exercise caution when using buffers of a fixed length (e.g. use sprintf_s).
	char formatString[32];

	// The buffer to be used with sprintf_s() when updating the HUD Text object.
	// (For example: "Score: 9001")
	// (NOTE: This buffer has an arbitrary length of 32 chars, which is sufficient for this project.
	//	Exercise caution when using buffers of a fixed length (e.g. use sprintf_s).
	char displayString[32];

	// The value currently being displayed by the HUD Text object.
	// (NOTE: This value can be compared with *watchValue to determine when the text must be updated.)
	// (NOTE: Make sure to update this value each time the text is updated.)
	int displayValue;

} BehaviorHudText, * BehaviorHudTextPtr;
//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
static void BehaviorHudTextUpdateText(BehaviorHudTextPtr);
//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new (Bullet) behavior component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
BehaviorPtr BehaviorHudTextCreate(void)
{
	BehaviorHudTextPtr newBehavior = calloc(1, sizeof(BehaviorHudText));
	if (newBehavior == NULL)
		return NULL;
	newBehavior->scoreSystemId = SsiInvalid;
	newBehavior->base.memorySize = sizeof(BehaviorHudText);
	newBehavior->base.onInit = BehaviorHudTextInit;
	newBehavior->base.onUpdate = BehaviorHudTextUpdate;
	newBehavior->base.onExit = BehaviorHudTextExit;
	newBehavior->base.stateCurr = cHudTextInvalid;
	newBehavior->base.stateNext = cHudTextIdle;

	return (BehaviorPtr)newBehavior;
}

// Read the properties of a Behavior component from a file.
// (NOTE: Read the base Behavior values using BehaviorRead.)
// (NOTE: Read the format string using StreamReadToken.)
// (NOTE: Read the Score System ID value using StreamReadInt.)
// Params:
//	 behavior = Pointer to the Behavior component.
//	 stream = Pointer to the data stream used for reading.
void BehaviorHudTextRead(BehaviorPtr behavior, Stream stream)
{
	if (behavior != NULL && stream != NULL )
	{
		BehaviorHudTextPtr hud = (BehaviorHudTextPtr)behavior;
		BehaviorRead(behavior, stream);
		const char * fString = StreamReadToken(stream);
		strcpy_s(hud->formatString, sizeof(hud->formatString), fString);
		hud->scoreSystemId = StreamReadInt(stream);
	}
}

// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorHudTextInit(BehaviorPtr behavior)
{
	BehaviorHudTextPtr hud = (BehaviorHudTextPtr)behavior;
	SpritePtr sprite = GameObjectGetSprite(behavior->parent);

	if (sprite != NULL)
	{
		SpriteSetText(sprite, hud->displayString);
	}
	BehaviorHudTextUpdateText(hud);
}

// Update the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorHudTextUpdate(BehaviorPtr behavior, float dt)
{
	UNREFERENCED_PARAMETER(dt);
	BehaviorHudTextPtr hud = (BehaviorHudTextPtr)behavior;
	if (hud->displayValue != ScoreSystemGetValue(hud->scoreSystemId))
	{
	   BehaviorHudTextUpdateText(hud);
	}
}

// Exit the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorHudTextExit(BehaviorPtr behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

static void BehaviorHudTextUpdateText(BehaviorHudTextPtr hud)
{
	if (hud->scoreSystemId != SsiInvalid)
	{
		hud->displayValue = ScoreSystemGetValue(hud->scoreSystemId);
		sprintf_s(hud->displayString, 32, hud->formatString, hud->displayValue);
	}
}