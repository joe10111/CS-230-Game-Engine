//------------------------------------------------------------------------------
//
// File Name:	Transform.c
// Author(s):	Joe Centeno(joe.centeno)
// Project:		Project 2
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Transform.h"
#include "AEEngine.h"
#include "Trace.h"
#include "Vector2D.h"
#include "Matrix2D.h"
#include "Stream.h"

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

typedef struct Transform
{
	// Translation
	Vector2D translation;

	// Scale
	Vector2D scale;

	// Rotation
	float rotation;

	Matrix2D matrix;

	bool isDirty;

} TransformComponet;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------
Vector2D scaleInit = {1, 1};
//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

TransformPtr TransformCreate(void)
{
	TransformComponet* TempPointer = calloc(1, sizeof(TransformComponet));

	if (TempPointer)
	{
		TempPointer->isDirty = true;
		TempPointer->scale = scaleInit;

		return TempPointer;
	}

	TraceMessage("ERROR: FAILED");

	return NULL;
}

TransformPtr TransformClone(const TransformPtr other)
{
	if (other)
	{
		TransformPtr tmpTrans = TransformCreate();
		*tmpTrans = *other;

		return tmpTrans;
	}

	TraceMessage("Error: TransformClone requires a valid Transform ptr");
	return NULL;
}


void TransformFree(TransformPtr* transform)
{
	if (transform)
	{
		free(*transform);
		*transform = NULL;

		return;
	}

	TraceMessage("ERROR: FAILED ");

	return;
}

void TransformRead(TransformPtr transform, Stream stream)
{
	if (transform && stream)
	{
		StreamReadVector2D(stream, &transform->translation);

		transform->rotation = StreamReadFloat(stream);

		StreamReadVector2D(stream, &transform->scale);

		return;
	}

	TraceMessage("ERROR: FAILED");

	return;
}

const Vector2D* TransformGetTranslation(const TransformPtr transform)
{
	if (transform)
	{
	  return &transform->translation;
	}
		
	TraceMessage("ERROR: FAILED");

	return NULL;
}

float TransformGetRotation(const TransformPtr transform)
{
	if (transform)
	{
		return transform->rotation;
	}
		
	TraceMessage("ERROR: FAILED");

	return (float)ZERO;
}

const Vector2D* TransformGetScale(const TransformPtr transform)
{
	if (transform)
	{
		return &transform->scale;
	}
		
	TraceMessage("ERROR: FAILED");

	return NULL;
}

void TransformSetTranslation(TransformPtr transform, const Vector2D* translation)
{
	if (transform && translation)
	{
		transform->translation = *translation;
		transform->isDirty = true;

		return;
	}

	TraceMessage("ERROR: FAILED");

	return;
}

void TransformSetRotation(TransformPtr transform, float rotation)
{
	if (transform)
	{
		transform->rotation = rotation;
		transform->isDirty = true;
		return;
	}

	TraceMessage("ERROR: FAILED");

	return;
}

void TransformSetScale(TransformPtr transform, const Vector2D* scale)
{
	if (transform && scale)
	{
		transform->scale = *scale;
		transform->isDirty = true;
		return;
	}

	TraceMessage("ERROR: FAILED");

	return;
}

Matrix2D* TransformGetMatrix(const TransformPtr transform)
{
	if (transform)
	{
		if (transform->isDirty)
		{
			Matrix2D* scale = (Matrix2D*)calloc(1, sizeof(Matrix2D));
			Matrix2D* rot = (Matrix2D*)calloc(1, sizeof(Matrix2D));
			Matrix2D* trans = (Matrix2D*)calloc(1, sizeof(Matrix2D));
			Matrix2D result = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

			Matrix2DScale(scale, transform->scale.x, transform->scale.y);
			Matrix2DRotRad(rot, transform->rotation);
			Matrix2DTranslate(trans, transform->translation.x, transform->translation.y);

			Matrix2DConcat(&result, rot, scale);
			Matrix2DConcat(&transform->matrix, trans, &result);
			transform->isDirty = false;
		}
		AEGfxSetTransform(transform->matrix.m);
		return &transform->matrix;
	}
	TraceMessage("Error: TransformGetMatrix requires a valid transform ptr");
	return NULL;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------