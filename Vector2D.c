//------------------------------------------------------------------------------
//
// File Name:	Vector2D.c
// Author(s):	Joe Centeno(joe.centeno)
// Project:		Project 2
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Vector2D.h"
#include "Trace.h"

#define _USE_MATH_DEFINES
#define ZERO 0.0f
#define ONE_EIGHTY 180.0f

#include <math.h>

//------------------------------------------------------------------------------
// Private Constants:
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

void Vector2DZero(Vector2D* pResult)
{
	if (pResult)
	{
		pResult->x = ZERO;
		pResult->y = ZERO;

		return;
	}

	TraceMessage("ERROR: REQUIRES VALID VECTOR 2D");

	return;
}

void Vector2DSet(Vector2D* pResult, float x, float y)
{
	if (pResult)
	{
		pResult->x = x;
		pResult->y = y;

		return;
	}

	TraceMessage("ERROR: REQUIRES VALID VECTOR 2D");

	return;
}

void Vector2DNeg(Vector2D* pResult, const Vector2D* pVec0)
{
	float tempXCord = pVec0->x;
	float tempYCord = pVec0->y;

	if (pVec0)
	{
		pResult->x = -tempXCord;
		pResult->y = -tempYCord;

		return;
	}

	TraceMessage("ERROR: REQUIRES VALID VECTOR 2D");

	return;
}

void Vector2DAdd(Vector2D* pResult, const Vector2D* pVec0, const Vector2D* pVec1)
{
	float tempX1Cord = pVec0->x;
	float tempX2Cord = pVec0->y;

	float tempY1Cord = pVec1->x;
	float tempY2Cord = pVec1->y;

	if ((pVec0) && (pVec1))
	{
		pResult->x = tempX1Cord + tempY1Cord;
		pResult->y = tempX2Cord + tempY2Cord;

		return;
	}

	TraceMessage("ERROR: REQUIRES VALID VECTOR 2D");

	return;
}

void Vector2DSub(Vector2D* pResult, const Vector2D* pVec0, const Vector2D* pVec1)
{
	float tempX1Cord = pVec0->x;
	float tempY1Cord = pVec0->y;

	float tempX2Cord = pVec1->x;
	float tempY2Cord = pVec1->y;

	if (pVec0 && pVec1)
	{
		pResult->x = tempX1Cord - tempX2Cord;
		pResult->y = tempY1Cord - tempY2Cord;

		return;
	}

	TraceMessage("ERROR: REQUIRES VALID VECTOR 2D");

	return;
}

void Vector2DNormalize(Vector2D* pResult, const Vector2D* pVec0)
{
	Vector2D tempVector2D = *pVec0;

	float tempXCord = pVec0->x;
	float tempYCord = pVec0->y;

	if (pVec0)
	{
		pResult->x = tempXCord / Vector2DLength(&tempVector2D);
		pResult->y = tempYCord / Vector2DLength(&tempVector2D);

		return;
	}

	TraceMessage("ERROR: REQUIRES VALID VECTOR 2D");

	return;
}

void Vector2DScale(Vector2D* pResult, const Vector2D* pVec0, float scale)
{
	float tempXCord = pVec0->x;
	float tempYCord = pVec0->y;

	if (pVec0)
	{
		pResult->x = tempXCord * scale;
		pResult->y = tempYCord * scale;

		return;
	}

	TraceMessage("ERROR: REQUIRES VALID VECTOR 2D");

	return;
}

void Vector2DScaleAdd(Vector2D* pResult, const Vector2D* pVec0, const Vector2D* pVec1, float scale)
{
	float tmpX1 = pVec0->x;
	float tmpY1 = pVec0->y;

	float tmpX2 = pVec1->x;
	float tmpY2 = pVec1->y;

	if (pVec0 && pVec1)
	{
		pResult->x = ((tmpX1 * scale) + tmpX2);
		pResult->y = ((tmpY1 * scale) + tmpY2);

		return;
	}

	TraceMessage("ERROR: REQUIRES TWO VALID VECTOR 2D");

	return;
}

void Vector2DScaleSub(Vector2D* pResult, const Vector2D* pVec0, const Vector2D* pVec1, float scale)
{
	float tempX1Cord = pVec0->x;
	float tempY1Cord = pVec0->y;

	float tempX2Cord = pVec1->x;
	float tempY2Cord = pVec1->y;

	if (pVec0 && pVec1)
	{
		pResult->x = ((tempX1Cord * scale) - tempX2Cord);
		pResult->y = ((tempY1Cord * scale) - tempY2Cord);

		return;
	}

	TraceMessage("ERROR: REQUIRES TWO VALID VECTOR 2D");

	return;
}

float Vector2DLength(const Vector2D* pVec0)
{
	float vectorLength = ZERO;

	if (pVec0)
	{
		vectorLength = Vector2DSquareLength(pVec0);

		return sqrtf(vectorLength);
	}

	TraceMessage("ERROR: REQUIRES VALID VECTOR 2D");

	return vectorLength;
}

float Vector2DSquareLength(const Vector2D* pVec0)
{
	if (pVec0)
		return (pVec0->x * pVec0->x) + (pVec0->y * pVec0->y);

	TraceMessage("ERROR: REQUIRES VALID VECTOR 2D");

	return ZERO;
}

float Vector2DDistance(const Vector2D* pVec0, const Vector2D* pVec1)
{
	if (pVec0)
		return sqrtf(((pVec1->x - pVec0->x) * (pVec1->x - pVec0->x)) + ((pVec1->y - pVec0->y) * (pVec1->y - pVec0->y)));

	TraceMessage("ERROR: REQUIRES VALID VECTOR 2D");

	return ZERO;
}

float Vector2DSquareDistance(const Vector2D* pVec0, const Vector2D* pVec1)
{
	if (pVec0)
		return ((pVec1->x - pVec0->x) * (pVec1->x - pVec0->x)) + ((pVec1->y - pVec0->y) * (pVec1->y - pVec0->y));
	
	TraceMessage("ERROR: REQUIRES VALID VECTOR 2D");

	return ZERO;
}

float Vector2DDotProduct(const Vector2D* pVec0, const Vector2D* pVec1)
{
	if (pVec0 && pVec1)
		return (pVec0->x * pVec1->x) + (pVec0->y * pVec1->y);
	
	TraceMessage("ERROR: REQUIRES VALID VECTOR 2D");

	return ZERO;
}

void Vector2DFromAngleDeg(Vector2D* pResult, float angle)
{
	pResult->x = cosf(angle * (float)M_PI / ONE_EIGHTY);
	pResult->y = sinf(angle * (float)M_PI / ONE_EIGHTY);

	return;
}

void Vector2DFromAngleRad(Vector2D* pResult, float angle)
{
	pResult->x = (float)cos(angle);
	pResult->y = (float)sin(angle);

	return;
}

float Vector2DToAngleRad(const Vector2D* pVec)
{
	if (pVec)
		return atan2f(pVec->y, pVec->x);
	
	TraceMessage("ERROR: REQUIRES VALID VECTOR 2D");

	return ZERO;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------