//------------------------------------------------------------------------------
//
// File Name:	GameStateLevel2.c
// Author(s):	Joe Centeno(joe.centeno)
// Project:		Project 2
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameStateManager.h"
#include "GameStateLevel2.h"
#include "Trace.h"
#include "Stream.h"
#include "Vector2D.h"
#include "Mesh.h"
#include "SpriteSource.h"
#include "Sprite.h"
#include "AEEngine.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "Transform.h"
#include "Physics.h"
#include <math.h>

#define ZERO 0

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

static const float spaceshipSpeed = 500.0f;

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

static AEGfxVertexList* pMeshSpaceship;
static GameObjectPtr objPtr;

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

static int numLives = 0;
static int numHealth = 0;
static Stream FileStream;
errno_t error;

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

void GameStateLevel2MovementController(GameObjectPtr gameObject);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Load the resources associated with the Level2 game state.
void GameStateLevel2Load()
{
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFF0000, 0.0f, 0.0f,
		0.5f, 0.0f, 0xFFFFFF00, 0.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFF0000, 0.0f, 0.0f);
	pMeshSpaceship = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMeshSpaceship, "Failed to create spaceship mesh!");
	 
}

// Initialize the memory associated with the Level2 game state.
void GameStateLevel2Init()
{
	objPtr = GameObjectFactoryBuild("SpaceshipHoming");
	if (objPtr)
	{
		SpritePtr tmpSpritePtr = GameObjectGetSprite(objPtr);
		SpriteSetMesh(tmpSpritePtr, pMeshSpaceship);
	}

	AEGfxSetBackgroundColor(0, 0, 0);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
}

void GameStateLevel2MovementController(GameObjectPtr gameObject)
{
	PhysicsPtr physPtr = GameObjectGetPhysics(gameObject);
	TransformPtr transformPtr = GameObjectGetTransform(gameObject);

	if (!physPtr && !transformPtr)
	{
		TraceMessage("Error: GameStateLevel2MovementController could not retrieve physics and transform pointers.");
		return;
	}

	s32 mousePosX;
	s32 mousePosY;

	Vector2D worldPos;

	AEInputGetCursorPosition(&mousePosX, &mousePosY);
	AEGfxConvertScreenCoordinatesToWorld((float)mousePosX, (float)mousePosY, &worldPos.x, &worldPos.y);

	const Vector2D* shipTranslationPtr = TransformGetTranslation(transformPtr);

	Vector2D dirVec;

	Vector2DSub(&dirVec, &worldPos, shipTranslationPtr);
	Vector2DNormalize(&dirVec, &dirVec);

	float angleRad = Vector2DToAngleRad(&dirVec);

	TransformSetRotation(transformPtr, angleRad);

	Vector2DScale(&dirVec, &dirVec, spaceshipSpeed);
	PhysicsSetVelocity(physPtr, &dirVec);
	return;
}

// Update the Level2 game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateLevel2Update(float dt)
{
	if (AEInputCheckCurr('Z'))
		SpriteSetAlpha(GameObjectGetSprite(objPtr), 0.5f);
	else if (AEInputCheckCurr('X'))
		SpriteSetAlpha(GameObjectGetSprite(objPtr), 1.0f);

	GameStateLevel2MovementController(objPtr);
	GameObjectUpdate(objPtr, dt);
	GameObjectDraw(objPtr);

	// Gamestate switching
	if (AEInputCheckCurr('1'))
	{
		GameStateManagerSetNextState(GsLevel1);
	}

	if (AEInputCheckCurr('2'))
	{
		GameStateManagerSetNextState(GsRestart);
	}

	if (AEInputCheckCurr('3'))
	{
		GameStateManagerSetNextState(GsAsteroids);
	}

	if (AEInputCheckTriggered('4'))
	{
		GameStateManagerSetNextState(GsOmega);
	}

	if (AEInputCheckCurr('9'))
	{
		GameStateManagerSetNextState(GsSandbox);
	}

	if (AEInputCheckCurr('0'))
	{
		GameStateManagerSetNextState(GsDemo);
	}
		
}

// UnloadSprite any memory associated with the Level2 game state.
void GameStateLevel2Shutdown()
{
	free(objPtr);
}

// Unload the resources associated with the Level2 game state.
void GameStateLevel2Unload()
{
	AEGfxMeshFree(pMeshSpaceship);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

