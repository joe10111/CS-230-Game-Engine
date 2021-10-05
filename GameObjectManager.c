//------------------------------------------------------------------------------
//
// File Name:	GameObjectManager.c
// Author(s):	Joe Centeno
// Project:		Project 4
// Course:		CS230SU21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameObjectManager.h"
#include "Collider.h"

#define OBJECT_LIST_SIZE 512
#define MAX_NAME_LENGTH 32

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct GameObjectManager
{
	unsigned int objectCount;

	unsigned int objectMax;

	GameObjectPtr objectList[OBJECT_LIST_SIZE];

} GameObjectManager;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

static GameObjectManager gameObjectActiveList;
static GameObjectManager gameObjectArchetypes;

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

void GameObjectManagerInit(void)
{
	memset(gameObjectActiveList.objectList, 0, sizeof(gameObjectActiveList.objectList));
	memset(gameObjectArchetypes.objectList, 0, sizeof(gameObjectArchetypes.objectList));
}

void GameObjectManagerUpdate(float dt)
{
	for (unsigned int i = 0; i < OBJECT_LIST_SIZE; i++)
	{
		GameObjectUpdate(gameObjectActiveList.objectList[i], dt);

		if (GameObjectIsDestroyed(gameObjectActiveList.objectList[i]))
		{
			GameObjectFree(&gameObjectActiveList.objectList[i]);
			gameObjectActiveList.objectCount--;
		}
		
	}
	return;
}

// Draw all active game objects
void GameObjectManagerDraw(void)
{
	for (unsigned int i = 0; i < OBJECT_LIST_SIZE; i++)
	{
		GameObjectDraw(gameObjectActiveList.objectList[i]);
	}
	return;
}

// Shut Down 
void GameObjectManagerShutdown(void)
{
	gameObjectActiveList.objectCount = 0;
	gameObjectArchetypes.objectCount = 0;

	for (int i = 0; i < OBJECT_LIST_SIZE; i++)
	{
		GameObjectFree(&gameObjectActiveList.objectList[i]);
		GameObjectFree(&gameObjectArchetypes.objectList[i]);		
	}

	return;
}

// Add a game object
void GameObjectManagerAdd(GameObjectPtr gameObject)
{
	gameObjectActiveList.objectCount++;
	unsigned int i = 0;
	for (; i < OBJECT_LIST_SIZE; i++)
	{
		if (!gameObjectActiveList.objectList[i])
		{
			break;
		}
	}

	gameObjectActiveList.objectList[i] = gameObject;

	return;
}

void GameObjectManagerAddArchetype(GameObjectPtr gameObject)
{
	gameObjectArchetypes.objectCount++;
	unsigned int i = 0;
	for (; i < OBJECT_LIST_SIZE; i++)
	{
		if (!gameObjectArchetypes.objectList[i])
		{
			break;
		}
	}

	gameObjectArchetypes.objectList[i] = gameObject;

	return;
}

GameObjectPtr GameObjectManagerGetObjectByName(const char* name)
{
	unsigned int i = 0;
	for (; i < OBJECT_LIST_SIZE; i++)
	{
		if (GameObjectIsNamed(gameObjectActiveList.objectList[i], name))
			return gameObjectActiveList.objectList[i];
	}

	return NULL;
}

GameObjectPtr GameObjectManagerGetArchetype(const char* name)
{
	unsigned int i = 0;
	for (; i < OBJECT_LIST_SIZE; i++)
	{
		if (GameObjectIsNamed(gameObjectArchetypes.objectList[i], name))
			return gameObjectArchetypes.objectList[i];
	}

	return NULL;
}

void GameObjectManagerCheckCollisions()
{
	for (unsigned i = 0; i < OBJECT_LIST_SIZE; i++)
	{
		if (gameObjectActiveList.objectList[i] != NULL)
		{
			ColliderPtr collider1 = GameObjectGetCollider(gameObjectActiveList.objectList[i]);

			if (collider1 != NULL)
			{
				for (unsigned j = i + 1; j < OBJECT_LIST_SIZE; ++j)
				{
					if (gameObjectActiveList.objectList[j] != NULL)
					{
						ColliderPtr collider2 = GameObjectGetCollider(gameObjectActiveList.objectList[j]);
						if (collider2 != NULL)
						{
							ColliderCheck(collider1, collider2);
						}
					}
				}
			}
		}
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------