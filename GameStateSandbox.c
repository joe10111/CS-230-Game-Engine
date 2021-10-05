//------------------------------------------------------------------------------
//
// File Name:	GameSandBox.c
// Author(s):	Joe Centeno(joe.centeno)
// Project:		Project 2
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "AEEngine.h"
#include "GameStateManager.h"
#include "GameStateSandbox.h"
#include "Stream.h"
#include "Trace.h"
#include "Vector2D.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

int SandBoxRun = 0;
//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

static Stream FileStream;
errno_t error;


//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

static void GameStateSandboxTraceFloat(const char* test, float f);

static void GameStateSandboxTraceVector(const char* text, const Vector2D* v);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Load the resources associated with the SandBox game state.
void GameStateSandboxLoad()
{
}

// Initialize the memory associated with the SandBox game state.
void GameStateSandboxInit()
{
}

// Update the SandBox game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateSandboxUpdate(float dt)
{
	/* Tell the compiler that the 'dt' variable is unused. */
	UNREFERENCED_PARAMETER(dt);

	FileStream = StreamOpen("./Data/VectorTests.txt");

	Vector2D vectorTest1;
	Vector2D vector1Test2;
	Vector2D vector2Test2;
	Vector2D vector3Test;

	// Change GameState with Keys : 1 : 2 : 9 : 0
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
		GameStateManagerSetNextState(GsAsteroids);
	}

	if (AEInputCheckTriggered('4'))
	{
		GameStateManagerSetNextState(GsOmega);
	}

	if (AEInputCheckCurr('9') && SandBoxRun == true)
	{
		GameStateManagerSetNextState(GsRestart);
	}

	if (AEInputCheckCurr('0'))
	{
		GameStateManagerSetNextState(GsDemo);
	}

	if (FileStream == NULL)
	{
		char errorBuffer[257];

		strerror_s(errorBuffer, sizeof errorBuffer, error);

		fprintf(stderr, "CANT OPEN FILE '%s' : %s\n",
			"Level1_Lives.txt", errorBuffer);
	}
	else
	{
		Vector2DZero(&vectorTest1);
		GameStateSandboxTraceVector("Vector2DZero", &vectorTest1);

		Vector2DSet(&vectorTest1, 1.0f, 1.5f);
		GameStateSandboxTraceVector("Vector2DSet", &vectorTest1);

		Vector2DNeg(&vectorTest1, &vectorTest1);
		GameStateSandboxTraceVector("Vector2DNeg", &vectorTest1);

		Vector2DAdd(&vectorTest1, &vectorTest1, &vectorTest1);
		GameStateSandboxTraceVector("Vector2DAdd", &vectorTest1);

		Vector2DSub(&vectorTest1, &vectorTest1, &vectorTest1);
		GameStateSandboxTraceVector("Vector2DSub", &vectorTest1);

		StreamReadVector2D(FileStream, &vectorTest1);
		GameStateSandboxTraceVector("v", &vectorTest1);

		Vector2DNormalize(&vectorTest1, &vectorTest1);
		GameStateSandboxTraceVector("Vector2DNormalize", &vectorTest1);

		float scale = StreamReadFloat(FileStream);
		GameStateSandboxTraceFloat("scale", scale);

		Vector2DScale(&vectorTest1, &vectorTest1, scale);
		GameStateSandboxTraceVector("Vector2DScale", &vectorTest1);

		Vector2DScaleAdd(&vectorTest1, &vectorTest1, &vectorTest1, scale);
		GameStateSandboxTraceVector("Vector2DScaleAdd", &vectorTest1);

		Vector2DScaleSub(&vectorTest1, &vectorTest1, &vectorTest1, scale);
		GameStateSandboxTraceVector("Vector2DScaleSub", &vectorTest1);

		Vector2DLength(&vectorTest1);
		GameStateSandboxTraceFloat("Vector2DLength", Vector2DLength(&vectorTest1));

		Vector2DSquareLength(&vectorTest1);
		GameStateSandboxTraceFloat("Vector2DSquareLength", Vector2DSquareLength(&vectorTest1));


		StreamReadVector2D(FileStream, &vector1Test2);
		GameStateSandboxTraceVector("p1", &vector1Test2);

		StreamReadVector2D(FileStream, &vector2Test2);
		GameStateSandboxTraceVector("p2", &vector2Test2);

		Vector2DDistance(&vector1Test2, &vector2Test2);
		GameStateSandboxTraceFloat("Vector2DDistance", Vector2DDistance(&vector1Test2, &vector2Test2));

		Vector2DSquareDistance(&vector1Test2, &vector2Test2);
		GameStateSandboxTraceFloat("Vector2DSquareDistance", Vector2DSquareDistance(&vector1Test2, &vector2Test2));

		Vector2DDotProduct(&vector1Test2, &vector2Test2);
		GameStateSandboxTraceFloat("Vector2DDotProduct", Vector2DDotProduct(&vector1Test2, &vector2Test2));


		float angle = StreamReadFloat(FileStream);
		GameStateSandboxTraceFloat("deg", angle);

		Vector2DFromAngleDeg(&vector3Test, angle);
		GameStateSandboxTraceVector("Vector2DFromAngleDeg", &vector3Test);

		angle = StreamReadFloat(FileStream);
		GameStateSandboxTraceFloat("rad", angle);

		Vector2DFromAngleRad(&vector3Test, angle);
		GameStateSandboxTraceVector("Vector2DFromAngleRad", &vector3Test);

		angle = Vector2DToAngleRad(&vector3Test);
		GameStateSandboxTraceFloat("Vector2DToAngleRad", angle);

		StreamClose(&FileStream);
	}

	SandBoxRun = 1;
}

// UnloadSprite any memory associated with the SandBox game state.
void GameStateSandboxShutdown()
{
	// UnloadSprite all objects.
}

// Unload the resources associated with the SandBox game state.
void GameStateSandboxUnload()
{
	// UnloadSprite all sprite sources.

	// Unload all textures.

	// UnloadSprite all meshes.
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

static void GameStateSandboxTraceFloat(const char* test, float f)
{
	TraceMessage("Vector Test: %s = %f", test, f);
}

static void GameStateSandboxTraceVector(const char* text, const Vector2D* v)
{
	TraceMessage("Vector Test: %s = [%f, %f]", text, v->x, v->y);
}
