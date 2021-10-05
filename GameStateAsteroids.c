//------------------------------------------------------------------------------
//
// File Name:	GameStateAstroids.c
// Author(s):	Joe Centeno(joe.centeno)
// Project:		Project 6
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "GameStateManager.h"
#include "GameStateAsteroids.h"
#include "MeshManager.h"
#include "AEEngine.h"
#include "GameObject.h"
#include "Mesh.h"
#include "SpriteSource.h"
#include "Sprite.h"
#include "GameObjectFactory.h"
#include "GameObjectManager.h"
#include "SpriteSourceManager.h"
#include "GameObject.h"
#include "Trace.h"
#include "Vector2D.h"
#include "BehaviorHudText.h"
#include "ScoreSystem.h"

//------------------------------------------------------------------------------
// Private Constants:
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

// Load the resources associated with the Asteroids game state.
void GameStateAsteroidsLoad()
{
	SpriteSourceManagerInit();
	MeshManagerInit();
	ScoreSystemInit();
}

// Initialize the memory associated with the Asteroids game state.
void GameStateAsteroidsInit()
{
	GameObjectPtr spaceship = GameObjectFactoryBuild("Spaceship");

	if (spaceship)
		GameObjectManagerAdd(spaceship);
	else
		TraceMessage("Error: GameStateAsteroid could not create space ship object");

	GameObjectManagerAdd(GameObjectFactoryBuild("AsteroidsScore"));
	GameObjectManagerAdd(GameObjectFactoryBuild("AsteroidsHighScore"));
	GameObjectManagerAdd(GameObjectFactoryBuild("AsteroidsWave"));

	//GameStateAsteroidsCreateHudElement("AsteroidsHighScore", "High Score: %d", &asteroidHighScore);
	//GameStateAsteroidsCreateHudElement("AsteroidsScore", "Score: %d", &asteroidScore);
    //GameStateAsteroidsCreateHudElement("AsteroidsWave", "Wave: %d", &asteroidWaveCount);

	
	asteroidSpawnCount = cAsteroidSpawnInitial;

	ScoreSystemRestart();
	GameStateAsteroidsSpawnAsteroidWave();

	AEGfxSetBackgroundColor(0, 0, 0);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
}

// Update the Asteroids game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateAsteroidsUpdate(float dt)
{
	/* Tell the compiler that the 'dt' variable is unused. */
	UNREFERENCED_PARAMETER(dt);

	if (!GameObjectManagerGetObjectByName("Asteroid"))
	{
		GameStateAsteroidsSpawnAsteroidWave();
	}
	
	// Gamestate switching
	if (AEInputCheckCurr('1'))
	{
		GameStateManagerSetNextState(GsLevel1);
	}

	if (AEInputCheckCurr('2'))
	{
		GameStateManagerSetNextState(GsLevel2);
	}
		
	if (AEInputCheckCurr('3'))
	{
		GameStateManagerSetNextState(GsRestart);
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

// Free any memory associated with the Asteroids game state.
void GameStateAsteroidsShutdown()
{
	// Free all objects.
	GameObjectManagerShutdown();
	MeshManagerFreeAll();
}

// Unload the resources associated with the Asteroids game state.
void GameStateAsteroidsUnload()
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