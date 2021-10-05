//------------------------------------------------------------------------------
//
// File Name:	Physics.c
// Author(s):	Joe Centeno(joe.centeno)
// Project:		Project 2
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------


#include "stdafx.h"    // Standard System Include Files
#include "Physics.h"   // Physics Headerfile
#include "Vector2D.h"  // Vector Math 
#include "Trace.h"     // Trace Log System
#include "Stream.h"    // Stream 
#include "Transform.h" // PhysicsComponent

//------------------------------------------------------------------------------
// Public Defines:
//------------------------------------------------------------------------------
#define ZERO 0
#define ONE 1

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct Physics
{
	Vector2D PrevPosition;    // Prev Position

	Vector2D acceleration;    // Acceleration 

	Vector2D velocity;	      // Velocity

	float rotationalVelocity; // Rotational velocity 

} PhysicsComponent;

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

// Createse A Physics Componet
PhysicsPtr PhysicsCreate(void)
{
	PhysicsComponent* PhysicsTemp = calloc(ONE, sizeof(PhysicsComponent));

	if (PhysicsTemp)
	{
		return PhysicsTemp;
	}
		
	TraceMessage("ERROR:PHYSICS CREATE");

	return NULL;
}

//Clones Physic Component 
PhysicsPtr PhysicsClone(const PhysicsPtr other)
{
	if (other)
	{
		PhysicsPtr NewPhysic = PhysicsCreate();
		*NewPhysic = *other;

		return NewPhysic;
	}
	else if (other == NULL)
	{
		//TraceMessage("Error: Physics Clone Requires a valid pointer");
		return NULL;
	}
	return NULL;
}

//Free Physics System
void PhysicsFree(PhysicsPtr* physics)
{
	if (physics)
	{
		free(*physics);

		*physics = NULL;

		return;
	}

	TraceMessage("ERROR:PHYSICS FREE");

	return;
}

//Read Physics Stream
void PhysicsRead(PhysicsPtr physics, Stream stream)
{
	if (stream)
	{
		StreamReadVector2D(stream, &physics->acceleration);
		StreamReadVector2D(stream, &physics->velocity);

		return;
	}

	TraceMessage("ERROR:PHYSICS READ STREAM");

	return;
}

//Get Acceleration
const Vector2D* PhysicsGetAcceleration(const PhysicsPtr physics)
{
	if (physics)
	{
		return &physics->acceleration;
	}

	TraceMessage("ERROR:PHYSICS GET ACCELERATION");

	return NULL;
}

//Get Velocity
const Vector2D* PhysicsGetVelocity(const PhysicsPtr physics)
{
	if (physics)
	{
		return &physics->velocity;
	}
		
	TraceMessage("ERROR:GET VELOCITY");

	return NULL;
}

 //Get the Rotational Velocity of a Physics Component
float PhysicsGetRotationalVelocity(PhysicsPtr physics)
{
	if (physics)
	{
	   return physics->rotationalVelocity;
	}

	TraceMessage("ERROR:GET ROTTIONAL VELOCITY");
	return 0.0f;
}

// Get Prev Position
const Vector2D* PhysicsGetOldTranslation(PhysicsPtr physics)
{
	if (physics)
	{
		return &physics->PrevPosition;
	}
		
	TraceMessage("ERROR:GET PREV POSition");

	return NULL;
}

//Set Acceleration
void PhysicsSetAcceleration(PhysicsPtr physics, const Vector2D* acceleration)
{
	if (physics && acceleration)
	{
		physics->acceleration = *acceleration;

		return;
	}

	TraceMessage("ERROR: SET ACCERLATION");

	return;
}

//Set VELOCITY
void PhysicsSetVelocity(PhysicsPtr physics, const Vector2D* velocity)
{
	if (physics && velocity)
	{
		physics->velocity = *velocity;
		return;
	}
	TraceMessage("Error: PhysicsSetVelocity Requires two valid pointers");
	return;
}

void PhysicsSetRotationalVelocity(PhysicsPtr physics, float rotationalVelocity)
{
	if (physics)
	{
		physics->rotationalVelocity = rotationalVelocity;

		return;
	}

	TraceMessage("Error: PhysicsSetVelocity Requires two valid arguments");
	return;

}

// Update Loop for Physicis
void PhysicsUpdate(PhysicsPtr physics, TransformPtr transform, float dt)
{
	if (physics && transform)
	{
		Vector2D Position = *TransformGetTranslation(transform);
		float rotPtr = TransformGetRotation(transform);

		physics->PrevPosition = Position;

		Vector2DScaleAdd(&physics->velocity, &physics->acceleration, &physics->velocity, dt);
		Vector2DScaleAdd(&Position, &physics->velocity, &Position, dt);

		TransformSetTranslation(transform, &Position);
		rotPtr += physics->rotationalVelocity * dt;
		TransformSetRotation(transform, rotPtr);
		return;
	}

	TraceMessage("ERROR: UPDATE");

	return;
}