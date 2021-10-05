#include "stdafx.h"
#include "Collider.h"
#include "Stream.h"
#include "Transform.h"
#include "GameObject.h"
#include "Vector2D.h"
#include "ColliderCircle.h"
#include "ColliderLine.h"

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
//static bool CollisionCircleCircle(float r1, float r2, double distance);
static bool ColliderIsColliding(ColliderPtr collider1, ColliderPtr collider2);
//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------


// Dynamically allocate a clone of an existing Collider component.
// (Hint: Perform a shallow copy of the member variables.)
// Params:
//	 other = Pointer to the component to be cloned.
// Returns:
//	 If 'other' is valid and the memory allocation was successful,
//	   then return a pointer to the cloned component,
//	   else return NULL.
ColliderPtr ColliderClone(const ColliderPtr other)
{
	if (other == NULL)
		return NULL;

	ColliderPtr clone = calloc(1, other->memorySize);

	if (clone == NULL)
		return NULL;

	memcpy_s(clone, other->memorySize, other, other->memorySize);
	
	clone->parent = NULL;

	return clone;
}

// Free the memory associated with a Collider component.
// (Also, set the collider pointer to NULL.)
// Params:
//	 collider = Pointer to the collider component.
void ColliderFree(ColliderPtr* collider)
{ 
	free(*collider);
	*collider = NULL;
}

// Read the properties of a Collider component from a file.
// [NOTE: No values need to be read at this time.]
// Params:
//	 collider = Pointer to the Collider component.
//	 stream = Pointer to the data stream used for reading.
void ColliderRead(ColliderPtr collider, Stream stream)
{
	if (collider && stream)
	{

		return;
	}


	return;
}

// Set the parent game object for a Collider component.
// Params:
//	 collider = Pointer to the Collider component.
//	 parent = Pointer to the parent game object.
void ColliderSetParent(ColliderPtr collider, GameObjectPtr parent)
{
	if(collider && parent)
	{
		collider->parent = parent;
		return;
	}

	return;
}

// Check if two objects are colliding.
// (Hint: Refer to the project instructions for implementation suggestions.)
// (Hint: Make sure to call the handler for both colliders, passing the 
//	  parent game object pointers in the correct order!)
// Params:
//	 collider1 = Pointer to the first collider component.
//	 collider2 = Pointer to the second collider component.
void ColliderCheck(ColliderPtr collider, ColliderPtr other)
{
	if (collider == NULL || other == NULL)
		return;

	if (ColliderIsColliding(collider, other))
	{
		if (collider->handler != NULL)
			collider->handler(collider->parent, other->parent);
		if (other->handler != NULL)
			other->handler(other->parent, collider->parent);
	}
	
	/*
	TransformPtr colliderTransform, otherTransform;

	const Vector2D* colliderTranslate;
	const Vector2D* otherTranslate;

	colliderTransform = GameObjectGetTransform(collider->parent);
	colliderTranslate = TransformGetTranslation(colliderTransform);

	otherTransform = GameObjectGetTransform(other->parent);
	otherTranslate = TransformGetTranslation(otherTransform);
	
	float r1 = TransformGetScale(colliderTransform)->x / 2.0f;
	float r2 = TransformGetScale(otherTransform)->x / 2.0f;

	double distance = Vector2DSquareDistance(colliderTranslate, otherTranslate);
	
	bool collistion = CollisionCircleCircle(r1, r2, distance);

	if (collistion)
	{
		if (collider->handler != NULL)
			collider->handler(collider->parent, other->parent);

		if (other->handler != NULL)
			other->handler(other->parent, collider->parent);
	}
	*/
}

// Set the collision event handler for a collider.
// (Hint: This allows other components, such as behaviors, to respond to collision events.)
// (Note: It is acceptable for the handler to be NULL.  This allows an existing handler to be removed.)
// Params:
//	 collider = Pointer to the collider component.
//	 handler = Pointer to the collision event handler (may be NULL).
void ColliderSetCollisionHandler(ColliderPtr collider, CollisionEventHandler handler)
{
	collider->handler = handler;
}

/*
static bool CollisionCircleCircle(float r1, float r2, double distance)
{ 
	//sqrt(((double)x1 - (double)x2) * ((double)x1 - (double)x2) + ((double)y1 - (double)y2) * ((double)y1 - (double)y2));

	double squaredRad = (r1 + r2) * (r1 + r2);

	if (distance <= squaredRad)
		return 1;
	else
		return 0;
}
*/

static bool ColliderIsColliding(ColliderPtr collider1, ColliderPtr collider2)
{
	if (collider1->type == ColliderTypeCircle && collider2->type == ColliderTypeCircle)
	{
		return ColliderCircleIsCollidingWithCircle(collider1, collider2);
	}
	else if (collider1->type == ColliderTypeLine && collider2->type == ColliderTypeLine)
	{
		return false;
	}
	// This and the last condition are the same, except swapped around.
	else if (collider1->type == ColliderTypeLine && collider2->type == ColliderTypeCircle)
	{
		return ColliderLineIsCollidingWithCircle(collider1, collider2);
	}
	else if (collider1->type == ColliderTypeCircle && collider2->type == ColliderTypeLine)
	{
		return ColliderLineIsCollidingWithCircle(collider2, collider1);
	}
	return false;
}