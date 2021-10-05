//------------------------------------------------------------------------------
//
// File Name:	Behvior.c
// Author(s):	Joe Centeno(joe.centeno)
// Project:		Project 2
// Course:		CS230SU21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "Behavior.h"
#include "Stream.h"
#include "Trace.h"

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

BehaviorPtr BehaviorClone(BehaviorPtr other)
{
	if (other)
	{
		BehaviorPtr clone = malloc(other->memorySize);

		memcpy_s(clone, other->memorySize, other, other->memorySize);

		return clone;
	}

	TraceMessage("Error: BehaviorClone requires a valid behavior pointer");
	return NULL;
}

void BehaviorFree(BehaviorPtr* behavior)
{
	if (behavior)
	{
		free(*behavior);
		*behavior = NULL;
		return;
	}

	return;
}

void BehaviorRead(BehaviorPtr behavior, Stream stream)
{
	if (behavior && stream)
	{
		behavior->stateCurr = StreamReadInt(stream);

		behavior->stateNext = StreamReadInt(stream);

		behavior->timer = StreamReadFloat(stream);
		return;
	}

	TraceMessage("Error: BehaviorRead Requires a valid behavior ptr and a valid stream");
	return;
}

void BehaviorSetParent(BehaviorPtr behavior, GameObjectPtr parent)
{
	if (behavior && parent)
	{
		behavior->parent = parent;
		return;
	}

	TraceMessage("Error: BehaviorSetParent requires a valid behavior ptr and a valid parent ptr");
	return;
}

void BehaviorUpdate(BehaviorPtr behavior, float dt)
{
	if (behavior == NULL)
	{
		TraceMessage("Error: BehaviorUpdate requires a valid Behavior ptr");
		return;
	}
		

	if (behavior->stateCurr != behavior->stateNext)
	{
		if (behavior->onExit)
		{
			behavior->onExit(behavior);
		}

		behavior->stateCurr = behavior->stateNext;

		if (behavior->onInit)
		{
			behavior->onInit(behavior);
		}
	}

	if (behavior->onUpdate)
	{
		behavior->onUpdate(behavior, dt);
	}

  
  return;
}
	

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
