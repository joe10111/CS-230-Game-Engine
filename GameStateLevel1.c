//------------------------------------------------------------------------------
//
// File Name:	GameStateLevel1.c
// Author(s):	Joe Centeno(joe.centeno)
// Project:		Project 2
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameStateManager.h"
#include "GameStateLevel1.h"
#include "Stream.h"
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
#include "Animation.h"

//------------------------------------------------------------------------------
// Public Defines:
//------------------------------------------------------------------------------
#define ZERO 0
#define ONE 1

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

static const float HeightGround = -150.0f;

static const float MovementVelocity = 500.0f;

static const float JumpVelocity = 1000.0f;

static const Vector2D gravityNormal = { 0.0f, -1500.0f };
static const Vector2D gravityNone = { 0.0f, 0.0f };

typedef enum MonkeyStates {
	MonkeyInvalid = -1,
	MonkeyIdle,
	MonkeyWalk,
	MonkeyJump,
} MonkeyStates;

static const float wallDistance = 350.0f;
static const float CheckSquareDistance = (75.0f * 75.0f);


//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------


static Stream FileStream;
errno_t error;

static AEGfxVertexList* MeshOne;
static AEGfxTexture* TextureOne;
static SpriteSourcePtr SpriteFilePath;
static GameObjectPtr Planet;
static int numLives1 = ZERO;

static AEGfxVertexList* mesh3x3;
static AEGfxVertexList* mesh1x1;
static AEGfxVertexList* mesh16x6;

static GameObjectPtr monkey;
static GameObjectPtr planet;

static AEGfxTexture* planetTexture;
static AEGfxTexture* monkeyIdleTexture;
static AEGfxTexture* monkeyWalkTexture;
static AEGfxTexture* monkeyJumpTexture;

static SpriteSourcePtr planetSpriteSource;
static SpriteSourcePtr monkeyIdleSpriteSource;
static SpriteSourcePtr monkeyWalkSpriteSource;
static SpriteSourcePtr monkeyJumpSpriteSource;

static AEGfxTexture* fontSheet;
static SpriteSourcePtr fontSheetSpriteSource;
static GameObjectPtr fontSheetObj;

static enum MonkeyStates monkeyState = MonkeyInvalid;
static char livesBuffer[16] = "";

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

void GameStateLevel1MovementController(GameObjectPtr gameObject);

void GameStateLevel1SetMonkeyState(GameObjectPtr gameObject, MonkeyStates newState);

void GameStateLevel1BounceController(GameObjectPtr gameObject);

bool GameStateLevel1IsColliding(GameObjectPtr obj1, GameObjectPtr obj2);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

//Load GameState
void GameStateLevel1Load()
{
	FileStream = StreamOpen("./Data/Level1_Lives.txt");

	if (FileStream == NULL)
	{
		char Buffer[257];

		strerror_s(Buffer, sizeof Buffer, error);

		fprintf(stderr,
			"CANT OPEN '%s' : %s\n",
			"Level1_Lives.txt", Buffer);
	}
	else
		numLives1 = StreamReadInt(FileStream);

	MeshOne = MeshCreateQuad(0.5f, 0.5f, 1.0f, 1.0f, "Mesh1x1");

	TextureOne = AEGfxTextureLoad("./Assets/PlanetTexture.png");

	SpriteFilePath = SpriteSourceCreate(1, 1, TextureOne);

	mesh1x1 = MeshCreateQuad(0.5f, 0.5f, 1.0f, 1.0f, "Mesh1x1");
	mesh3x3 = MeshCreateQuad(0.5, 0.5, 1.0f / 3, 1.0f / 3, "Mesh3x3");
	mesh16x6 = MeshCreateQuad(0.5, 0.5, 1.0f / 16, 1.0f / 6, "Mesh16x6");

	monkeyIdleTexture = AEGfxTextureLoad("./Assets/MonkeyIdle.png");
	monkeyWalkTexture = AEGfxTextureLoad("./Assets/MonkeyWalk.png");
	monkeyJumpTexture = AEGfxTextureLoad("./Assets/MonkeyJump.png");
	fontSheet = AEGfxTextureLoad("./Assets/FontSheet.png");

	monkeyIdleSpriteSource = SpriteSourceCreate(1, 1, monkeyIdleTexture);
	monkeyWalkSpriteSource = SpriteSourceCreate(3, 3, monkeyWalkTexture);
	monkeyJumpSpriteSource = SpriteSourceCreate(1, 1, monkeyJumpTexture);

	fontSheetSpriteSource = SpriteSourceCreate(16, 6, fontSheet);

	planetTexture = AEGfxTextureLoad("./Assets/PlanetTexture.png");
	planetSpriteSource = SpriteSourceCreate(1, 1, planetTexture);
}

// Initialize GameState
void GameStateLevel1Init()
{
	planet = GameObjectFactoryBuild("PlanetBounce");
	if (planet)
	{
		SpritePtr tmpSpritePtr = GameObjectGetSprite(planet);
		SpriteSetMesh(tmpSpritePtr, mesh1x1);
		SpriteSetSpriteSource(tmpSpritePtr, planetSpriteSource);
	}
	monkey = GameObjectFactoryBuild("Monkey");
	if (monkey)
	{
		monkeyState = MonkeyInvalid;
		GameStateLevel1SetMonkeyState(monkey, MonkeyIdle);
	}
	fontSheetObj = GameObjectFactoryBuild("MonkeyLivesText");
	if (fontSheet)
	{
		SpritePtr tmpSpritePtr = GameObjectGetSprite(fontSheetObj);
		SpriteSetMesh(tmpSpritePtr, mesh16x6);
		SpriteSetSpriteSource(tmpSpritePtr, fontSheetSpriteSource);
		sprintf_s(livesBuffer, sizeof(livesBuffer), "Lives: %d", numLives1);
		SpriteSetText(tmpSpritePtr, livesBuffer);
	}

	AEGfxSetBackgroundColor(1, 1, 1);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
}

//Planet Controller 
void GameStateLevel1MovementController(GameObjectPtr gameObject)
{

	PhysicsPtr PhysicsPointer = GameObjectGetPhysics(gameObject);

	TransformPtr TransformPointer = GameObjectGetTransform(gameObject);

	if (!PhysicsPointer || !TransformPointer)
	{
		TraceMessage("ERROR: PLANET CONTROLLER");

		return;
	}

	Vector2D* Velocity = (Vector2D*)PhysicsGetVelocity(PhysicsPointer);


	if (AEInputCheckCurr(VK_LEFT))
	{
		Velocity->x = -MovementVelocity;

		if (monkeyState != MonkeyJump)
			GameStateLevel1SetMonkeyState(gameObject, MonkeyWalk);
	}
	else if (AEInputCheckCurr(VK_RIGHT))
	{
		Velocity->x = MovementVelocity;

		if (monkeyState != MonkeyJump)
			GameStateLevel1SetMonkeyState(gameObject, MonkeyWalk);
	}
	else
	{
		Velocity->x = 0;
		if (monkeyState != MonkeyJump)
			GameStateLevel1SetMonkeyState(gameObject, MonkeyIdle);
	}

	if (AEInputCheckTriggered(VK_UP))
	{
		Velocity->y = JumpVelocity;

		PhysicsSetAcceleration(PhysicsPointer, &gravityNormal);
		GameStateLevel1SetMonkeyState(gameObject, MonkeyJump);
	}

	Vector2D* Position = (Vector2D*)TransformGetTranslation(TransformPointer);
	if (Position->y < HeightGround)
	{
		Position->y = HeightGround;
		Velocity->y = 0;
		GameStateLevel1SetMonkeyState(gameObject, MonkeyIdle);
		PhysicsSetAcceleration(PhysicsPointer, &gravityNone);
	}

	return;
}

//Update
void GameStateLevel1Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
	GameStateLevel1MovementController(monkey);
	GameStateLevel1BounceController(planet);
	GameObjectUpdate(monkey, dt);
	GameObjectUpdate(planet, dt);
	GameObjectUpdate(fontSheetObj, dt);



	GameObjectDraw(monkey);
	GameObjectDraw(planet);
	GameObjectDraw(fontSheetObj);


	if (GameStateLevel1IsColliding(monkey, planet))
	{
		numLives1--;
		if (numLives1 <= 0)
			GameStateManagerSetNextState(GsLevel2);
		else
			GameStateManagerSetNextState(GsRestart);
	}
	// Change GameState with Keys : 1 : 2 : 9 : 0
	if (AEInputCheckCurr('1'))
	{
		GameStateManagerSetNextState(GsRestart);
	}

	if (AEInputCheckCurr('2'))
	{
		GameStateManagerSetNextState(GsLevel2);
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

// Free Level 1 Data & Shutdown
void GameStateLevel1Shutdown()
{
	// Free
	if (FileStream != NULL)
	{
		fclose(FileStream);
		free(Planet);
		free(monkey);
		free(fontSheetObj);
	}
	else
		TraceMessage("ERROR: CANT CLOSE %s;", FileStream);

}

// Unload the resources associated with the Stub game state.
void GameStateLevel1Unload()
{
	// Free Sprite.
	free(SpriteFilePath);

	// Unload Textures.
	AEGfxTextureUnload(TextureOne);

	// Free Mesh
	AEGfxMeshFree(MeshOne);

	//free all sprite sources.
	free(monkeyWalkSpriteSource);
	free(monkeyJumpSpriteSource);
	free(fontSheetSpriteSource);
	free(planetSpriteSource);

	// Unload all textures.
	AEGfxTextureUnload(planetTexture);
	AEGfxTextureUnload(monkeyIdleTexture);
	AEGfxTextureUnload(monkeyWalkTexture);
	AEGfxTextureUnload(monkeyJumpTexture);


	//free all meshes.
	AEGfxMeshFree(mesh3x3);
	AEGfxMeshFree(mesh1x1);
	AEGfxMeshFree(mesh16x6);
}

void GameStateLevel1SetMonkeyState(GameObjectPtr gameObject, MonkeyStates newState)
{
	if (monkeyState != newState)
	{
		monkeyState = newState;
		AnimationPtr gameObjAnimation = GameObjectGetAnimation(gameObject);

		switch (newState)
		{
		case MonkeyIdle:
			SpriteSetMesh(GameObjectGetSprite(gameObject), mesh1x1);
			SpriteSetSpriteSource(GameObjectGetSprite(gameObject), monkeyIdleSpriteSource);
			AnimationPlay(gameObjAnimation, 1, 0.0f, false);
			break;

		case MonkeyWalk:
			SpriteSetMesh(GameObjectGetSprite(gameObject), mesh3x3);
			SpriteSetSpriteSource(GameObjectGetSprite(gameObject), monkeyWalkSpriteSource);
			AnimationPlay(gameObjAnimation, 8, 0.05f, true);
			break;

		case MonkeyJump:
			SpriteSetMesh(GameObjectGetSprite(gameObject), mesh1x1);
			SpriteSetSpriteSource(GameObjectGetSprite(gameObject), monkeyJumpSpriteSource);
			AnimationPlay(gameObjAnimation, 1, 0.0f, false);
			break;
		}
	}
}

void GameStateLevel1BounceController(GameObjectPtr gameObject)
{
	PhysicsPtr objPhys = GameObjectGetPhysics(gameObject);
	TransformPtr objTrans = GameObjectGetTransform(gameObject);

	if (objPhys && objTrans)
	{
		Vector2D pos = { TransformGetTranslation(objTrans)->x, TransformGetTranslation(objTrans)->y };
		Vector2D vel = { PhysicsGetVelocity(objPhys)->x, PhysicsGetVelocity(objPhys)->y };

		if (pos.x <= -wallDistance)
		{
			pos.x = -wallDistance;
			vel.x = -vel.x;
		}
		if (pos.x >= wallDistance)
		{
			pos.x = wallDistance;
			vel.x = -vel.x;
		}
		if (pos.y <= HeightGround)
		{
			pos.y = HeightGround + (HeightGround - pos.y);
			vel.y = -vel.y;
		}

		TransformSetTranslation(objTrans, &pos);
		PhysicsSetVelocity(objPhys, &vel);
		return;
	}
	TraceMessage("Error: GameStateLevel1BounceController requires a valic game object with valid physics and transform components");
	return;
}

bool GameStateLevel1IsColliding(GameObjectPtr obj1, GameObjectPtr obj2)
{
	TransformPtr obj1Trans = GameObjectGetTransform(obj1);
	TransformPtr obj2Trans = GameObjectGetTransform(obj2);

	const Vector2D* pos1 = TransformGetTranslation(obj1Trans);
	const Vector2D* pos2 = TransformGetTranslation(obj2Trans);

	float dist = Vector2DSquareDistance(pos1, pos2);

	if (dist < CheckSquareDistance)
		return true;
	else
		return false;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------