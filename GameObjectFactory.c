//------------------------------------------------------------------------------
//
// File Name:	GameObjectFactory.c
// Author(s):	Joe Centeno(joe.centeno)
// Project:		Project 2
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameObjectFactory.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "Stream.h"
#include "Trace.h"

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

GameObjectPtr GameObjectFactoryBuild(const char* objectName)
{
	if (objectName)
	{
		GameObjectPtr archetype = GameObjectManagerGetArchetype(objectName);

		if (!archetype)
		{
			char pathName[FILENAME_MAX] = "";
			sprintf_s(pathName, _countof(pathName), "Data/%s.txt", objectName);

			Stream fileStream = StreamOpen(pathName);

			if (fileStream)
			{
				const char* tmp = StreamReadToken(fileStream);
				
				if (strstr("GameObject", tmp))
				{
					archetype = GameObjectCreate();
					GameObjectRead(archetype, fileStream);
					GameObjectManagerAddArchetype(archetype);
				}

				StreamClose(&fileStream);
			}
			else
			{
				TraceMessage("Error: GameObjectFactoryBuild Could not create filestream");
				return NULL;
			}
		}
		
		if(archetype)
		{
			GameObjectPtr tmpGamObj = GameObjectClone(archetype);
			return tmpGamObj;
		}
	}

	TraceMessage("Error: GameObjectFactoryBuild expects a valid objectname");
	return NULL;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------