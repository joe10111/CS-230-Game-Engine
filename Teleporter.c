//------------------------------------------------------------------------------
//
// File Name:	Teleporter.c
// Author(s):	Joe Centeno (joe.centeno)
// Project:		Project 5
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Teleporter.h"
#include "GameObject.h"
#include "Transform.h"
#include "Vector2D.h"
#include <AEEngine.h>
#include "Physics.h"

// When a game object goes off-screen, teleport it to the other side.
// Params:
//	 gameObject = Pointer to the game object to be checked.
void TeleporterUpdateObject(GameObjectPtr gameObject)
{
	if (gameObject)
	{
		if(gameObject == NULL)
		  return; // If GameObject is Null Return

		// Get Transform of Object 
		TransformPtr transform = GameObjectGetTransform(gameObject);
		if (transform == NULL)
			return; // If Transform is Null Return

		// Get Physics of gameobject to Check Velocity
		PhysicsPtr physics = GameObjectGetPhysics(gameObject);
		if (physics == NULL)
			return; // If Physics is Null Return

		Vector2D const* vel = PhysicsGetVelocity(physics);        // Velocity of GameObject
		Vector2D const* pos = TransformGetTranslation(transform); // Position of GameObject

		Vector2D telePos = { 0, 0 }; // Init Teleport Position
		
		if (vel->x > 0.0f) // If we are moving on the X 
		{
			if (pos->x > AEGfxGetWinMaxX())    // If our current X position is greater then Max X
				telePos.x = AEGfxGetWinMinX(); // Set teleport position to the oppsit side of screen 
		}
		else
		{
			if (pos->x < AEGfxGetWinMinX())    // If our current X position is less then then Min X
				telePos.x = AEGfxGetWinMaxX(); // Set teleport position to the oppsit side of screen 
		}

		if (vel->y > 0.0f) // If we are moving on the Y 
		{
			if (pos->y > AEGfxGetWinMaxY())    // If our current Y position is greater then Max Y
				telePos.y = AEGfxGetWinMinY(); // Set teleport position to the oppsit side of screen
		}
		else
		{
			if (pos->y < AEGfxGetWinMinY())     // If our current Y position is less then then Min Y
				telePos.y = AEGfxGetWinMaxY();  // Set teleport position to the oppsit side of screen
		}
		
		// If we need to teleport 
		if (telePos.x != 0 || telePos.y != 0)
		{
			// Set Transform to Teleport
			TransformSetTranslation(transform, &telePos);
		}
	}
	return; // If GameObject is Not Valid Return
}