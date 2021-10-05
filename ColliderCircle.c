//------------------------------------------------------------------------------
//
// File Name:	ScoreSystem.c
// Author(s):	Joe Centeno (joe.centeno)
// Project:		Project 6
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Stub.h"

#include "Collider.h"
#include "Vector2D.h"
#include "Transform.h"
#include "GameObject.h"
#include "Stream.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
typedef struct ColliderCircle
{
	// Inherit the base collider structure.
	Collider	base;

	// Radius of the circle collider.
	float radius;

} ColliderCircle, * ColliderCirclePtr;
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

// Dynamically allocate a new (circle) collider component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// (Hint: Make sure to initialize the ColliderType and memorySize correctly.)
ColliderPtr ColliderCircleCreate(void)
{
	ColliderCirclePtr circleCollider = calloc(1, sizeof(ColliderCircle));

	if (circleCollider == NULL)
		return NULL;

	circleCollider->base.memorySize = sizeof(ColliderCircle);
	circleCollider->base.type = ColliderTypeCircle;
	return (ColliderPtr)circleCollider;
}

// Get the circle collider's radius.
// Params:
//	 collider = Pointer to the circle collider component.
// Returns:
//	 If the pointer is valid,
//	   then return the circle collider's radius,
//	   else return 0.0f.
float ColliderCircleGetRadius(ColliderPtr collider)
{
	if (collider == NULL)
		return 0.0f;
	else
	{
		ColliderCirclePtr circle = (ColliderCirclePtr)(collider);
		return circle->radius;
	}
}

// Read the properties of a ColliderCircle component from a file.
// (NOTE: Read the radius value using StreamReadFloat.)
// Params:
//	 collider = Pointer to the Collider component.
//	 stream = Pointer to the data stream used for reading.
void ColliderCircleRead(ColliderPtr collider, Stream stream)
{
	if (collider != NULL && stream != NULL)
	{
		ColliderCirclePtr circle = (ColliderCirclePtr)(collider);
		circle->radius = StreamReadFloat(stream);
	}
}

// Set the circle collider's radius.
// Params:
//	 collider = Pointer to the circle collider component.
//   radius = the circle collider's new radius.
void ColliderCircleSetRadius(ColliderPtr collider, float radius)
{
	if (collider == NULL)
		return;
	ColliderCirclePtr circle = (ColliderCirclePtr)(collider);
	circle->radius = radius;
}

// Check for collision between two circle colliders.
// Params:
//	 collider1 = Pointer to the first circle collider component.
//	 collider2 = Pointer to the second circle collider component.
bool ColliderCircleIsCollidingWithCircle(ColliderPtr collider, ColliderPtr other)
{
	ColliderCirclePtr thisCircle = (ColliderCirclePtr)(collider);
	ColliderCirclePtr otherCircle = (ColliderCirclePtr)(other);

	// Check for null
	if (thisCircle == NULL || otherCircle == NULL)
		return false;

	// Check if both are circle
	if (collider->type != ColliderTypeCircle && other->type != ColliderTypeCircle)
		return false;

	// Get the distance squared between them
	TransformPtr thisTransform, otherTransform;
	thisTransform = GameObjectGetTransform(thisCircle->base.parent);
	otherTransform = GameObjectGetTransform(otherCircle->base.parent);
	float distanceSquared = Vector2DSquareDistance(TransformGetTranslation(thisTransform), TransformGetTranslation(otherTransform));

	// Get the sum of their radius, then sqaure it.
	float radius = thisCircle->radius + otherCircle->radius;
	float radiusSquare = radius * radius;

	// Compare the two.
	if (radiusSquare > distanceSquared)
	{
		// We have collided.
		if (collider->handler != NULL)
			collider->handler(collider->parent, other->parent);
		if (other->handler != NULL)
			other->handler(other->parent, collider->parent);
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------