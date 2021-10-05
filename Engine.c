//------------------------------------------------------------------------------
//
// File Name:	Engine.c
// Author(s):	Doug Schilling (dschilling)
// Project:		Project 0
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Engine.h"
#include "GameObjectManager.h"
#include "GameStateManager.h"
#include "System.h"
#include "Trace.h"
#include "Random.h"

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

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Initialize the game engine.
void EngineInit()
{
	//--------------------------------------------------------------------------
	// NOTE: Certain modules need to be initialized first.
	//--------------------------------------------------------------------------

	// Initialize the System (Windows, Event Handlers).
	SystemInit();

	// Initialize the Tracing/Logging module.
	TraceInit();

	TraceMessage("Engine: Init");

	//--------------------------------------------------------------------------
	// NOTE: Other modules can be initialized later and in any order.
	//--------------------------------------------------------------------------

	// Initialize the game state manager.
	GameStateManagerInit();
	GameObjectManagerInit();
	
	RandomInit();
}

// Update the game engine.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void EngineUpdate(float dt)
{
	TraceMessage("Engine: Update");

	// Update the System (Windows, Event Handlers).
	SystemUpdate(dt);

	// Update the game state manager.
	GameStateManagerUpdate(dt);
	GameObjectManagerUpdate(dt);
	GameObjectManagerCheckCollisions();
	GameObjectManagerDraw();

	// Complete the draw process for the current game loop.
	SystemDraw();
}

// Shutdown the game engine.
void EngineShutdown()
{
	TraceMessage("Engine: Shutdown");

	//--------------------------------------------------------------------------
	// NOTE: Some modules can be shutdown in any order.
	//--------------------------------------------------------------------------

	// Shutdown the game state manager.
	GameStateManagerShutdown();
	GameObjectManagerShutdown();
	//--------------------------------------------------------------------------
	// NOTE: Certain modules need to be shutdown last and in reverse order.
	//--------------------------------------------------------------------------

	// Shutdown the Tracing/Logging module.
	TraceShutdown();

	// Shutdown the System (Windows, Event Handlers).
	SystemShutdown();
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

