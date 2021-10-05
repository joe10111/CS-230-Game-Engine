//------------------------------------------------------------------------------
//
// File Name:	GameStateOmega.c
// Author(s):	Joe Centeno(joe.centeno)
// Project:		Project 6
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include <AEEngine.h>
#include "GameStateManager.h"
#include "GameStateOmega.h"
#include "Trace.h"
#include "GameStateManager.h"
#include "GameObjectManager.h"
#include "SpriteSourceManager.h"
#include "Vector2D.h"
#include "GameObjectFactory.h"
#include "MeshManager.h"
#include "ScoreSystem.h"
#include "SpriteSource.h"
#include "BehaviorHudText.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------
static const int cAsteroidSpawnInitial = 8;
static const int cAsteroidSpawnMaximum = 20;
//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------
static int asteroidSpawnCount;

static AEGfxVertexList* mesh16x6;
static char livesBuffer[16] = "";


//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
static void GameStateAsteroidsSpawnAsteroidWave(void);
static void GameStateAsteroidsSpawnAsteroid(void);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Load the resources associated with the Omega game state.
void GameStateOmegaLoad()
{
	SpriteSourceManagerInit();
	MeshManagerInit();
	ScoreSystemInit();
}

// Initialize the memory associated with the Omega game state.
void GameStateOmegaInit()
{
	GameObjectPtr spaceshipOmega = GameObjectFactoryBuild("SpaceshipOmega");

	if (spaceshipOmega)
		GameObjectManagerAdd(spaceshipOmega);
	else
		TraceMessage("Error: GameStateAsteroid could not create spaceshipOmega object");

	GameObjectManagerAdd(GameObjectFactoryBuild("Arena"));

	GameObjectPtr score = GameObjectFactoryBuild("OmegaScore");

	if (score)
	{
		GameObjectManagerAdd(score);
	}
	
	BehaviorHudTextInit(GameObjectGetBehavior(score));
	BehaviorHudTextUpdate(GameObjectGetBehavior(score), (float)ScoreSystemGetValue(SsiScore));

	GameObjectManagerAdd(GameObjectFactoryBuild("OmegaHighScore"));
	GameObjectManagerAdd(GameObjectFactoryBuild("OmegaWave"));

	asteroidSpawnCount = cAsteroidSpawnInitial;

	ScoreSystemRestart();
	GameStateAsteroidsSpawnAsteroidWave();

	AEGfxSetBackgroundColor(0, 0, 0);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
}

// Update the Omega game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateOmegaUpdate(float dt)
{
	/* Tell the compiler that the 'dt' variable is unused. */
	UNREFERENCED_PARAMETER(dt);


	if (!GameObjectManagerGetObjectByName("Asteroid"))
	{
		GameStateAsteroidsSpawnAsteroidWave();
	}

	if (AEInputCheckTriggered('1'))
	{
		GameStateManagerSetNextState(GsLevel1);
	}
	else if (AEInputCheckTriggered('2'))
	{
		GameStateManagerSetNextState(GsLevel2);
	}
	else if (AEInputCheckTriggered('3'))
	{
		GameStateManagerSetNextState(GsAsteroids);
	}
	else if (AEInputCheckTriggered('4'))
	{
		GameStateManagerSetNextState(GsRestart);
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

// Free any memory associated with the Omega game state.
void GameStateOmegaShutdown()
{
	GameObjectManagerShutdown();
	MeshManagerFreeAll();
}

// Unload the resources associated with the Omega game state.
void GameStateOmegaUnload()
{
	SpriteSourceManagerFreeAll();
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

static void GameStateAsteroidsSpawnAsteroidWave(void)
{
	for (int i = 0; i < asteroidSpawnCount; i++)
	{
		GameStateAsteroidsSpawnAsteroid();
	}

	asteroidSpawnCount++;

	// Limit it.
	if (asteroidSpawnCount > cAsteroidSpawnMaximum)
	{
		asteroidSpawnCount = cAsteroidSpawnMaximum;
	}

	ScoreSystemIncreaseWaveCount();
}

static void GameStateAsteroidsSpawnAsteroid(void)
{
	GameObjectPtr Asteroid = GameObjectFactoryBuild("Asteroid");
	if (Asteroid)
		GameObjectManagerAdd(Asteroid);
	else
		TraceMessage("Error: GameStateAsteroid could not create Asteroid object");
}
