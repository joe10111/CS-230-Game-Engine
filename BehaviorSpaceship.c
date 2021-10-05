//------------------------------------------------------------------------------
//
// File Name:	BehaviorSpaceShip.c
// Author(s):	Joe Centeno(joe.centeno)
// Project:		Project 4
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
#include "stdafx.h"
#include "BehaviorSpaceship.h"
#include "Behavior.h"
#include "Trace.h"
#include <AEEngine.h>
#include "Physics.h"
#include "GameObject.h"
#include "Transform.h"
#include "Vector2D.h"
#include "GameObjectFactory.h"
#include "GameObjectManager.h"
#include "Collider.h"
#include "Teleporter.h"
#include "GameStateManager.h"

#define PI 3.1415926f

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------
static const float spaceshipAcceleration = 150.0f;
static const float spaceshipSpeedMax = 500.0f;
static const float spaceshipTurnRateMax = PI / 1.5f;
static const float spaceshipWeaponCooldownTime = 0.25f;
static const float spaceshipWeaponBulletSpeed = 750.0f;
static const float spaceshipDeathDuration = 3.0f;

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

enum ShipStates {
	cSpaceshipInvalid = -1,
	cSpaceshipIdle,
	cSpaceshipThrust,
	cSpaceshipDead,
};


//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

static void BehaviorSpaceshipUpdateRotation(BehaviorPtr behavior, float dt);
static void BehaviorSpaceshipUpdateVelocity(BehaviorPtr behavior, float dt);
static void BehaviorSpaceshipUpdateWeapon(BehaviorPtr behavior, float dt);
static void BehaviorSpaceshipSpawnBullet(BehaviorPtr behavior);
static void BehaviorSpaceshipCollisionHandler(GameObjectPtr gameObject1, GameObjectPtr gameObject2);
static void BehaviorSpaceshipDeadAnim(BehaviorPtr behavior, float dt);


//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new (Spaceship) behavior component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
BehaviorPtr BehaviorSpaceshipCreate(void)
{
	BehaviorPtr tmpBeh = (BehaviorPtr)calloc(1, sizeof(Behavior));
	if (tmpBeh)
	{
		tmpBeh->stateCurr = cSpaceshipInvalid;
		tmpBeh->stateNext = cSpaceshipInvalid;

		tmpBeh->onInit = &BehaviorSpaceshipInit;
		tmpBeh->onUpdate = &BehaviorSpaceshipUpdate;
		tmpBeh->onExit = &BehaviorSpaceshipExit;
		tmpBeh->memorySize = sizeof(Behavior);

		return tmpBeh;
	}
	TraceMessage("Error: BehaviorSpaceShipCreate could not create a behavior ptr");
	return NULL;
}

// Initialize the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
void BehaviorSpaceshipInit(BehaviorPtr behavior)
{
	if (behavior->stateCurr == cSpaceshipIdle)
	{
		ColliderPtr collider = GameObjectGetCollider(behavior->parent);
		ColliderSetCollisionHandler(collider, BehaviorSpaceshipCollisionHandler);
	}
	else if (behavior->stateCurr == cSpaceshipDead)
	{
		behavior->timer = spaceshipDeathDuration;
	}

	UNREFERENCED_PARAMETER(behavior);
}

// Update the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorSpaceshipUpdate(BehaviorPtr behavior, float dt)
{
	switch (behavior->stateCurr)
	{
	case cSpaceshipIdle:
		BehaviorSpaceshipUpdateRotation(behavior, dt);
		BehaviorSpaceshipUpdateWeapon(behavior, dt);
		if (AEInputCheckCurr(VK_UP))
			behavior->stateNext = cSpaceshipThrust;
		break;
	case cSpaceshipThrust:
		BehaviorSpaceshipUpdateRotation(behavior, dt);
		BehaviorSpaceshipUpdateVelocity(behavior, dt);
		BehaviorSpaceshipUpdateWeapon(behavior, dt);
		if (!AEInputCheckCurr(VK_UP))
			behavior->stateNext = cSpaceshipIdle;
		break;

	case cSpaceshipDead:
		behavior->timer -= dt;
		if (behavior->timer < 0)
			GameStateManagerSetNextState(GsRestart);
		BehaviorSpaceshipDeadAnim(behavior, dt);
		break;
	}
	TeleporterUpdateObject(behavior->parent);
}

// Exit the current state of the behavior component.
// (Hint: Refer to the lecture notes on finite state machines (FSM).)
// Params:
//	 behavior = Pointer to the behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorSpaceshipExit(BehaviorPtr behavior)
{
	UNREFERENCED_PARAMETER(behavior);
}

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
static void BehaviorSpaceshipUpdateRotation(BehaviorPtr behavior, float dt)
{
	UNREFERENCED_PARAMETER(dt);
	PhysicsPtr tmpPhys = GameObjectGetPhysics(behavior->parent);
	if (AEInputCheckCurr(VK_LEFT))
		PhysicsSetRotationalVelocity(tmpPhys, spaceshipTurnRateMax);
	else if (AEInputCheckCurr(VK_RIGHT))
		PhysicsSetRotationalVelocity(tmpPhys, -spaceshipTurnRateMax);
	else
		PhysicsSetRotationalVelocity(tmpPhys, 0);
	return;
}

static void BehaviorSpaceshipUpdateVelocity(BehaviorPtr behavior, float dt)
{
	PhysicsPtr tmpPhys = GameObjectGetPhysics(behavior->parent);
	TransformPtr tmpTrans = GameObjectGetTransform(behavior->parent);

	if (!tmpPhys || !tmpTrans)
	{
		TraceMessage("Error: BehaviorSpaceshipUpdateVelocity requires a valid behavior ptr with valid physics and transform components");
		return;
	}

	Vector2D rotationVec = { 0, 0 };
	float tmpRot = TransformGetRotation(tmpTrans);
	Vector2DFromAngleRad(&rotationVec, tmpRot);
	//Vector2DFromAngle(&rotationVec, tmpRot);

	Vector2D velocity = *PhysicsGetVelocity(tmpPhys);
	Vector2DScaleAdd(&velocity, &rotationVec, &velocity, spaceshipAcceleration * dt);
	float speed = Vector2DLength(&velocity);
	if (speed > spaceshipSpeedMax)
		Vector2DScale(&velocity, &velocity, spaceshipSpeedMax / speed);
	PhysicsSetVelocity(tmpPhys, &velocity);
}

static void BehaviorSpaceshipUpdateWeapon(BehaviorPtr behavior, float dt)
{
	if (behavior->timer > 0)
	{
		behavior->timer -= dt;
		if (behavior->timer < 0)
			behavior->timer = 0;
	}
	if (AEInputCheckCurr(VK_SPACE))
	{
		if (behavior->timer <= 0)
		{
			behavior->timer = spaceshipWeaponCooldownTime;
			BehaviorSpaceshipSpawnBullet(behavior);
		}
	}
	return;
}

static void BehaviorSpaceshipSpawnBullet(BehaviorPtr behavior)
{
	GameObjectPtr bulletObj = GameObjectFactoryBuild("Bullet");
	GameObjectManagerAdd(bulletObj);
	if (bulletObj)
	{
		TransformPtr shipTrans = GameObjectGetTransform(behavior->parent);
		Vector2D shipPos = *TransformGetTranslation(shipTrans);
		float shipRot = TransformGetRotation(shipTrans);

		TransformPtr bulletTrans = GameObjectGetTransform(bulletObj);
		TransformSetTranslation(bulletTrans, &shipPos);
		TransformSetRotation(bulletTrans, shipRot);

		Vector2D rotationVec = { 0, 0 };
		Vector2DFromAngleRad(&rotationVec, shipRot);

		PhysicsPtr bulletPhys = GameObjectGetPhysics(bulletObj);
		Vector2DScale(&rotationVec, &rotationVec, spaceshipWeaponBulletSpeed);
		PhysicsSetVelocity(bulletPhys, &rotationVec);
	}
	return;
}

static void BehaviorSpaceshipCollisionHandler(GameObjectPtr gameObject1, GameObjectPtr gameObject2)
{
	if (gameObject1 != NULL && gameObject2 != NULL)
	{
		
		if (GameObjectIsNamed(gameObject2, "Asteroid"))
		{
			GameObjectGetBehavior(gameObject1)->stateNext = cSpaceshipDead;
		}
	}
}

static void BehaviorSpaceshipDeadAnim(BehaviorPtr behavior, float dt)
{
	UNREFERENCED_PARAMETER(dt);

	// Make Space Ship Spin 
	PhysicsPtr physics = GameObjectGetPhysics(behavior->parent);
	PhysicsSetRotationalVelocity(physics, PhysicsGetRotationalVelocity(physics) + 10);

	// Make the space Ship Get smaller over time
	TransformPtr transform = GameObjectGetTransform(behavior->parent);
	Vector2D newScale = *TransformGetScale(transform);
	Vector2DScale(&newScale, &newScale, 0.9f);
	TransformSetScale(transform, &newScale);
}