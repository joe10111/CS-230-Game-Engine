//------------------------------------------------------------------------------
//
// File Name:	GameObjectFactory.c
// Author(s):	Joe Centeno(joe.centeno)
// Project:		Project 2
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//

#include "stdafx.h"
#include "BehaviorBullet.h"
#include "Behavior.h"
#include "Trace.h"
#include "GameObject.h"
#include "Collider.h"
#include "Teleporter.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

enum BulletStates {
	cBulletInvalid = -1,
	cBulletIdle,
};

// Max Speed of the Bullet
static const float bulletSpeedMax = 500.0f;

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

static void BehaviorBulletUpdateLifeTimer(BehaviorPtr behavior, float dt);
static void BehaviorBulletCollisionHandler(GameObjectPtr gameObject1, GameObjectPtr gameObject2);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new (Bullet) behavior component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
BehaviorPtr BehaviorBulletCreate(void)
{
	BehaviorPtr bullet = (BehaviorPtr)calloc(1, sizeof(Behavior));
	if (bullet)
	{
		bullet->stateCurr = cBulletInvalid;
		bullet->stateNext = cBulletInvalid;
		bullet->onInit = &BehaviorBulletInit;
		bullet->onUpdate = &BehaviorBulletUpdate;
		bullet->onExit = &BehaviorBulletExit;
		bullet->memorySize = sizeof(Behavior);

		return bullet;
	}
	TraceMessage("Error: BehaviorBulletCreate could not create a behavior ptr");
	return NULL;
}

// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorBulletInit(BehaviorPtr behavior)
{
	if (behavior->stateCurr == cBulletIdle)
	{
		ColliderPtr parentCollider = GameObjectGetCollider(behavior->parent);

		if (parentCollider != NULL)
		{
			ColliderSetCollisionHandler(parentCollider, BehaviorBulletCollisionHandler);
		}
	}
	UNREFERENCED_PARAMETER(behavior);
}

// Update the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorBulletUpdate(BehaviorPtr behavior, float dt)
{
	switch (behavior->stateCurr)
	{
	case cBulletIdle:
		BehaviorBulletUpdateLifeTimer(behavior, dt);
		break;
	}

	TeleporterUpdateObject(behavior->parent);
}

// Exit the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorBulletExit(BehaviorPtr behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
static void BehaviorBulletUpdateLifeTimer(BehaviorPtr behavior, float dt)
{
	if (behavior->timer > 0)
	{
		behavior->timer -= dt;
		if (behavior->timer <= 0)
		{
			GameObjectDestroy(behavior->parent);
		}
	}
}

static void BehaviorBulletCollisionHandler(GameObjectPtr gameObject1, GameObjectPtr gameObject2)
{
	if (gameObject1 != NULL && gameObject2 != NULL)
	{
		if (GameObjectIsNamed(gameObject2, "Asteroid"))
		{
			GameObjectDestroy(gameObject1);
		}
	}
}