
#include "stdafx.h"
#include "Stub.h"

#include <AEEngine.h>
#include "Collider.h"
#include "ColliderLine.h"
#include "ColliderCircle.h"
#include "Vector2D.h"
#include "Transform.h"
#include "Physics.h"
#include "GameObject.h"
#include "Matrix2D.h"
#include "Trace.h"
#include "Stream.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------
#define cLineSegmentMax 100

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct ColliderLineSegment
{
	// A single line segment (P0 and P1).
	Vector2D	point[2];
} ColliderLineSegment;

typedef struct CollisionData
{
	Vector2D collisionPoint;
	Vector2D reflectionVector;
	Vector2D newEndPoint;
}CollisionData, * CollisionDataPtr;

typedef struct ColliderLine
{
	// Inherit the base collider structure.
	Collider	base;

	// The number of line segments in the list.
	unsigned int		lineCount;

	// The individual line segments.
	// (NOTE: Make sure to allocate enough memory for all line segments!)
	ColliderLineSegment	lineSegments[cLineSegmentMax];

} ColliderLine, * ColliderLinePtr;
//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
static bool PointLineIntersection(Vector2D const* circlePosition, Vector2D const* velocity,
	ColliderLineSegment const* segment, float radius, Vector2D* point);
static CollisionData CaclCollisionResponse(Vector2D const* circlePosition, Vector2D const* velocity,
	ColliderLineSegment const* segment, Vector2D const* collisionPoint);
static bool CircleLineIntersection(Vector2D const* circlePosition, Vector2D const* velocity, ColliderLineSegment const* segment,
	float radius, Vector2D* point);
//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new (line) collider component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// (Hint: Make sure to initialize the ColliderType and memorySize correctly.)
ColliderPtr ColliderLineCreate(void)
{
	ColliderLinePtr lineCollider = calloc(1, sizeof(ColliderLine));

	if (lineCollider == NULL)
		return NULL;

	lineCollider->base.memorySize = sizeof(ColliderLine);
	lineCollider->base.type = ColliderTypeLine;
	return (ColliderPtr)lineCollider;
}

// Read the properties of a ColliderLine component from a file.
// (NOTE: First read an integer indicating the number of lines to be read.)
// (NOTE: For each line, read P0 and P1 using StreamReadVector2D.)
// (HINT: Call ColliderLineAddLineSegment() to add each line.)
// Params:
//	 collider = Pointer to the Collider component.
//	 stream = Pointer to the data stream used for reading.
void ColliderLineRead(ColliderPtr collider, Stream stream)
{
	if (collider != NULL && stream != NULL)
	{
		ColliderLinePtr line = (ColliderLinePtr)(collider);
		line->lineCount = StreamReadInt(stream);

		//for loop
		  //make p0 on the stack
		  //make p1 on the stack
		  //stream read for p0 and p1
		  //call collider line add line segment
		//end loop

		for (unsigned int i = 0; i <= line->lineCount; i++)
		{
			 Vector2D p0;
			 Vector2D p1;

			StreamReadVector2D(stream, &p0);
			StreamReadVector2D(stream, &p1);

			ColliderLineAddLineSegment(collider, &p0, &p1);
			
			
		}
	}
}

// Add a line segment to the line collider's line segment list.
// Params:
//	 collider = Pointer to the line collider component.
//	 p0 = The line segment's starting position.
//	 p1 = The line segment's ending position.
void ColliderLineAddLineSegment(ColliderPtr collider, const Vector2D* p0, const Vector2D* p1)
{
	if (collider == NULL)
		return;

	if ((p0->x == 0 && p0->y == 0) && (p1->x == 0 && p1->y == 0))
		return;

	if (collider->type != ColliderTypeLine)
		return;

	ColliderLinePtr line = (ColliderLinePtr)(collider);

	line->lineSegments[line->lineCount].point[0] = *p0;
	line->lineSegments[line->lineCount].point[1] = *p1;

	line->lineCount++;
}

// Check for collision between a line collider and a circle collider.
// Params:
//	 collider1 = Pointer to the line collider component.
//	 collider2 = Pointer to the circle collider component.
// Returns:
//	 If the pointers are valid,
//	   then return the results of a line-circle collision check,
//	   else return false.
bool ColliderLineIsCollidingWithCircle(ColliderPtr collider, ColliderPtr other)
{
	if (collider == NULL || other == NULL)
		return false;
	if (collider->type != ColliderTypeLine || other->type != ColliderTypeCircle)
		return false;

	bool returnValue = false;

	// Get the translations of the colliders.
	TransformPtr thisTransform, otherTransform;
	thisTransform = GameObjectGetTransform(collider->parent);
	otherTransform = GameObjectGetTransform(other->parent);

	// We need to access each line segment.
	ColliderLinePtr lineCollider = (ColliderLinePtr)collider;

	// Get the radius of the circle collider.
	float radius = ColliderCircleGetRadius(other);

	Vector2D collisionPoint;
	Vector2D otherTranslation = *TransformGetTranslation(otherTransform);

	// Get the velocity of my circle object.
	Vector2D velocity = *PhysicsGetVelocity(GameObjectGetPhysics(other->parent));

	// Scale the velocity by DT
	Vector2DScale(&velocity, &velocity, (float)AEFrameRateControllerGetFrameTime());

	// For each line segment, check if it collides.
	for (unsigned i = 0; i < lineCollider->lineCount; i++)
	{
		// Check for collision
		if (PointLineIntersection(&otherTranslation, &velocity, &lineCollider->lineSegments[i],
			radius, &collisionPoint))
		{
			// Collision response
			CollisionData data = CaclCollisionResponse(&otherTranslation, &velocity, &lineCollider->lineSegments[i], &collisionPoint);

			// Set the circle to the new end point, and give it a new velocity.
			TransformSetTranslation(otherTransform, &data.newEndPoint);

			PhysicsPtr physics = GameObjectGetPhysics(other->parent);
			// Get my previous speed.
			float speed = Vector2DLength(PhysicsGetVelocity(physics));

			// Scale my unit vector of reflection to speed.
			Vector2D newVelocity;
			Vector2DScale(&newVelocity, &data.reflectionVector, speed);

			// Turn the object so that its facing its new reflected vector
			float rotationVector = Vector2DToAngleRad(&data.reflectionVector);
			TransformSetRotation(otherTransform, rotationVector);

			// Set the new velocity.
			PhysicsSetVelocity(physics, &newVelocity);

			returnValue = true;
		}
	}
	return returnValue;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
#pragma warning(push)
#pragma warning(disable :4505)  /* Ignore the function not used. */
static bool PointLineIntersection(Vector2D const* circlePosition, Vector2D const* velocity, ColliderLineSegment const* segment,
	float radius, Vector2D* point)
{
	// Get the vector AB
	Vector2D AB;
	Vector2DSub(&AB, &segment->point[0], &segment->point[1]);

	// Get the point if there is no collision
	Vector2D Be;
	Vector2DAdd(&Be, circlePosition, velocity);

	// Get the normal vector of the line segment.
	Vector2D normalVector;
	normalVector.x = AB.y;
	normalVector.y = -AB.x;

	// Normalize it
	Vector2DNormalize(&normalVector, &normalVector);

	// ti = (n * P0 - n * Bs) + D
	//		-----------------
	//			n * v

	float numerator = Vector2DDotProduct(&normalVector, &segment->point[0]) - Vector2DDotProduct(&normalVector, circlePosition);
	float denominator = Vector2DDotProduct(&normalVector, velocity);

	UNREFERENCED_PARAMETER(radius);

	float time = (numerator) / denominator;

	// Check if t is between 0 and 1
	if (0 <= time && time <= 1)
	{
		// We might have an intersection. Check for collision point.
		Vector2D movementWithTime;
		Vector2DScale(&movementWithTime, velocity, time);
		Vector2DAdd(point, &movementWithTime, circlePosition);

		// Check for if its an infinite wall collision that is happening
		Vector2D left, BA;
		Vector2DNeg(&BA, &AB);

		// (Bi - P0) * (P1 - P0) < 0
		Vector2DSub(&left, point, &segment->point[0]);
		if (Vector2DDotProduct(&left, &BA) < 0)
		{
			return false;
		}

		// Reuse the left varaible.
		// (Bi - P1) * ( P0 - P1 ) < 0
		Vector2DSub(&left, point, &segment->point[1]);
		Vector2DNeg(&BA, &AB);
		if (Vector2DDotProduct(&left, &AB) < 0)
		{
			return false;
		}
		return true;
	}
	else
	{
		// no intersection
		return false;
	}
}

static bool StaticPointInStaticCircle(Vector2D const* circlePosition, float radius, Vector2D const* point)
{
	float disSquared = Vector2DSquareDistance(circlePosition, point);
	if (disSquared < radius * radius)
		return true;
	return false;
}

static bool CircleLineIntersection(Vector2D const* circlePosition, Vector2D const* velocity, ColliderLineSegment const* segment,
	float radius, Vector2D* point)
{
	// Check if the circle is in either ends of the line segment
	if (StaticPointInStaticCircle(circlePosition, radius, &segment->point[0]))
	{
		// Let the collision point be the segment + radius in the direction of circle
		Vector2D pointToCircle;
		Vector2DSub(&pointToCircle, &segment->point[0], circlePosition);

		// Normalize it, then extend it by radius
		Vector2DNormalize(&pointToCircle, &pointToCircle);
		Vector2DScale(&pointToCircle, &pointToCircle, radius);

		Vector2DAdd(point, &segment->point[0], &pointToCircle);

		TraceMessage("No OUTS. Collision is true.");
		return true;
	}
	if (StaticPointInStaticCircle(circlePosition, radius, &segment->point[1]))
	{
		// Let the collision point be the segment + radius in the direction of circle
		Vector2D pointToCircle;
		Vector2DSub(&pointToCircle, &segment->point[0], circlePosition);

		// Normalize it, then extend it by radius
		Vector2DNormalize(&pointToCircle, &pointToCircle);
		Vector2DScale(&pointToCircle, &pointToCircle, radius);

		Vector2DAdd(point, &segment->point[0], &pointToCircle);
		TraceMessage("No OUTS. Collision is true.");
		return true;
	}

	// Get the vector AB
	Vector2D AB;
	Vector2DSub(&AB, &segment->point[0], &segment->point[1]);

	// Get the point if there is no collision
	Vector2D Be;
	Vector2DAdd(&Be, circlePosition, velocity);

	// Get the normal vector of the line segment.
	Vector2D normalVector;
	normalVector.x = AB.y;
	normalVector.y = -AB.x;

	// Normalize it
	Vector2DNormalize(&normalVector, &normalVector);

	// ti = (n * P0 - n * Bs) + D
	//		-----------------
	//			n * v

	float numerator = Vector2DDotProduct(&normalVector, &segment->point[0]) - Vector2DDotProduct(&normalVector, circlePosition);
	float denominator = Vector2DDotProduct(&normalVector, velocity);

	float D = 0;
	// If positive

	if (denominator > 0)
	{
		D = -radius;
	}
	else  // Negative
	{
		D = radius;
	}

	float time = (numerator + D) / denominator;

	// Check if t is between 0 and 1
	if (0 <= time && time <= 1)
	{
		// We might have an intersection. Check for collision point.
		Vector2D movementWithTime;
		Vector2DScale(&movementWithTime, velocity, time);
		Vector2DAdd(point, &movementWithTime, circlePosition);

		// Check for if its an infinite wall collision that is happening
		Vector2D left, BA;
		Vector2DNeg(&BA, &AB);

		TraceMessage("I might have collided. Time is %2f", time);

		// (Bi - P0) * (P1 - P0) < 0
		Vector2DSub(&left, point, &segment->point[0]);
		if (Vector2DDotProduct(&left, &BA) < 0)
		{
			TraceMessage("OUT 1. Value is %2f", Vector2DDotProduct(&left, &BA));
			return false;
		}

		// Reuse the left varaible.
		// (Bi - P1) * ( P0 - P1 ) < 0
		Vector2DSub(&left, point, &segment->point[1]);
		if (Vector2DDotProduct(&left, &AB) < 0)
		{
			TraceMessage("OUT 2. Value is %2f", Vector2DDotProduct(&left, &AB));
			return false;
		}

		// n * Bs - n * P0 < -r && n * Be - n * P0 < -r
		// Reuse left, make a new right.
		float leftSide, rightSide;
		leftSide = Vector2DDotProduct(&normalVector, circlePosition) - Vector2DDotProduct(&normalVector, &segment->point[0]);
		rightSide = Vector2DDotProduct(&normalVector, &Be) - Vector2DDotProduct(&normalVector, &segment->point[0]);
		if (leftSide < -radius && rightSide < -radius)
		{
			TraceMessage("OUT 3.");
			return false;
		}

		// n * Bs - n * P0 > r && n * Be - n * P0 > r
		if (leftSide > radius && rightSide > radius)
		{
			TraceMessage("OUT 4.");
			return false;
		}
		TraceMessage("No OUTS. Collision is true.");
		return true;
	}
	else
	{
		// no intersection
		return false;
	}
}
#pragma warning(pop)

static CollisionData CaclCollisionResponse(Vector2D const* circlePosition, Vector2D const* velocity, ColliderLineSegment const* segment,
	Vector2D const* collisionPoint)
{
	// Get the vector AB
	Vector2D AB;
	Vector2DSub(&AB, &segment->point[0], &segment->point[1]);

	// Get the normal vector of the line segment.
	Vector2D normalVector;
	normalVector.x = AB.y;
	normalVector.y = -AB.x;
	Vector2DNormalize(&normalVector, &normalVector);

	// This is the point if collision did not happen
	Vector2D Be;
	Vector2DAdd(&Be, velocity, circlePosition);

	// i = Be - Bi, Bi is point of collision, Be is depth penetration, 
	Vector2D i;
	Vector2DSub(&i, &Be, collisionPoint);

	// r = i - 2(i * n) n, r is reflection vector
	Vector2D r;
	float iDOTn = Vector2DDotProduct(&i, &normalVector) * 2;
	Vector2D reflectNormal;
	Vector2DScale(&reflectNormal, &normalVector, iDOTn);
	Vector2DSub(&r, &i, &reflectNormal);

	// Find the new point after reflection.
	Vector2D resultPoint;
	Vector2DAdd(&resultPoint, collisionPoint, &r);

	// Find the new velocity, r normalized.
	Vector2DNormalize(&r, &r);

	// Prepare the struct to send out.
	CollisionData data;
	data.collisionPoint = *collisionPoint;
	data.reflectionVector = r;
	data.newEndPoint = resultPoint;

	return data;
}