//------------------------------------------------------------------------------
//
// File Name:	BehaviorAstroids.c
// Author(s):	Joe Centeno (joe.centeno)
// Project:		Project 6
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "Transform.h"
#include "Physics.h"
#include "BehaviorAsteroid.h"
#include "Behavior.h"
#include "Random.h"
#include "Collider.h"
#include "GameObject.h"
#include "Vector2D.h"
#include "Teleporter.h"
#include "GameStateAsteroids.h"
#include "ScoreSystem.h"
#include <AEEngine.h>

#define ONE 1
//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------
static const float asteroidSpeedMin = 50.0f;
static const float asteroidSpeedMax = 100.0f;
static const float asteroidSpawnOffset = 10.0f;
//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
typedef enum
{
	cAsteroidLarge,
	cAsteroidMedium,
	cAsteroidSmall,

} AsteroidSize;

typedef enum
{
	cAsteroidInvalid = -1,
	cAsteroidIdle
} AsteroidStates;

typedef enum
{
	cAsteroidOriginTlc,
	cAsteroidOriginTrc,
	cAsteroidOriginBlc,
	cAsteroidOriginBrc,
	cAsteroidOriginCount,

} AsteroidOrigin;

typedef struct BehaviorAsteroid
{
	// Inherit the base behavior structure.
	Behavior	base;

	// Add asteroid-specific behavior variables.
	AsteroidSize	size;
	AsteroidOrigin	origin;

} BehaviorAsteroid, * BehaviorAsteroidPtr;
//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
static void BehaviorAsteroidSetPosition(BehaviorAsteroidPtr);
static void BehaviorAsteroidSetVelocity(BehaviorAsteroidPtr);
static void BehaviorAsteroidCollisionHandler(GameObjectPtr, GameObjectPtr);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new (Asteroid) behavior component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
BehaviorPtr BehaviorAsteroidCreate(void)
{
	BehaviorAsteroid* AsteroidPTR = calloc(1, sizeof(BehaviorAsteroid));
	if (AsteroidPTR == NULL)
		return NULL;
	AsteroidPTR->base.onInit = BehaviorAsteroidInit;
	AsteroidPTR->base.onUpdate = BehaviorAsteroidUpdate;
	AsteroidPTR->base.onExit = BehaviorAsteroidExit;
	AsteroidPTR->base.memorySize = sizeof(BehaviorAsteroid);
	AsteroidPTR->size = cAsteroidLarge;
	AsteroidPTR->base.stateCurr = cAsteroidInvalid;
	AsteroidPTR->base.stateNext = cAsteroidIdle;
	return (BehaviorPtr)AsteroidPTR;
}

// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorAsteroidInit(BehaviorPtr behavior)
{
	BehaviorAsteroid* AsteroidPTR = (BehaviorAsteroid*)behavior;

	if (AsteroidPTR->base.stateCurr == cAsteroidIdle)
	{
		AsteroidPTR->origin = RandomRange(0, 3);
		BehaviorAsteroidSetPosition(AsteroidPTR);
		BehaviorAsteroidSetVelocity(AsteroidPTR);

		ColliderPtr collider = GameObjectGetCollider(behavior->parent);
		if (collider != NULL)
		{
			ColliderSetCollisionHandler(collider, BehaviorAsteroidCollisionHandler);
		}
	}
}

// Update the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorAsteroidUpdate(BehaviorPtr behavior, float dt)
{
	UNREFERENCED_PARAMETER(dt);
	switch (behavior->stateCurr)
	{
	case cAsteroidIdle:

		break;
	default:
		break;
	}
	TeleporterUpdateObject(behavior->parent);
}

// Exit the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorAsteroidExit(BehaviorPtr behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
#include "Trace.h"
static void BehaviorAsteroidSetPosition(BehaviorAsteroidPtr obj)
{
	if (obj->size == cAsteroidLarge)
	{
		// Zeroed new position.
		Vector2D newPosition;
		Vector2DZero(&newPosition);

		switch (obj->origin)
		{
		case cAsteroidOriginTlc:
			newPosition.x = AEGfxGetWinMinX();
			newPosition.y = AEGfxGetWinMaxY();
			break;
		case cAsteroidOriginTrc:
			newPosition.x = AEGfxGetWinMaxX();
			newPosition.y = AEGfxGetWinMaxY();
			break;
		case cAsteroidOriginBlc:
			newPosition.x = AEGfxGetWinMinX();
			newPosition.y = AEGfxGetWinMinY();
			break;
		case cAsteroidOriginBrc:
			newPosition.x = AEGfxGetWinMaxX();
			newPosition.y = AEGfxGetWinMinY();
			break;
		default:
			break;
		}

	  TransformSetTranslation(GameObjectGetTransform(obj->base.parent), &newPosition);
	}
}

static void BehaviorAsteroidSetVelocity(BehaviorAsteroidPtr obj)
{
	float newAngle = 0;
	if (obj->size == cAsteroidLarge)
	{

		switch (obj->origin)
		{
		case cAsteroidOriginTlc:
			//newAngle = RandomRangeFloat(-10, -80);
			newAngle = RandomRangeFloat(-80, -10);
			break;
		case cAsteroidOriginTrc:
			newAngle = RandomRangeFloat(-100, -170);
			break;
		case cAsteroidOriginBlc:
			newAngle = RandomRangeFloat(10, 80);
			break;
		case cAsteroidOriginBrc:
			newAngle = RandomRangeFloat(100, 170);
			break;
		default:
			break;
		}
	}
	else if (obj->size == cAsteroidMedium || obj->size == cAsteroidSmall)
	{
		newAngle = RandomRangeFloat(0, 359);
	}
	float newSpeed = RandomRangeFloat(asteroidSpeedMin, asteroidSpeedMax);
	PhysicsPtr physics = GameObjectGetPhysics(obj->base.parent);

	if (physics != NULL)
	{
		Vector2D newVelocity;
		// Get the degree in vector form.
		Vector2DFromAngleDeg(&newVelocity, newAngle);
		// Normalize it
		Vector2DNormalize(&newVelocity, &newVelocity);
		// Scale it so its speed is that.
		Vector2DScale(&newVelocity, &newVelocity, newSpeed);
		// Set the new velocity.
		PhysicsSetVelocity(physics, &newVelocity);
	}
}

static void BehaviorAsteroidCollisionHandler(GameObjectPtr gameObject1, GameObjectPtr gameObject2)
{
	if (gameObject1 != NULL && gameObject2 != NULL)
	{
		if (GameObjectIsNamed(gameObject2, "Spaceship") || GameObjectIsNamed(gameObject2, "Bullet"))
		{
			ScoreSystemIncreaseScore(10);
			GameObjectDestroy(gameObject1);
		}
	}
}
