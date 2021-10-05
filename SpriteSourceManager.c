//------------------------------------------------------------------------------
//
// File Name:	SpriteManger.c
// Author(s):	Joe Centeno (joe.centeno)
// Project:		Project 5
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "SpriteSourceManager.h"
#include "SpriteSource.h"
#include "Stream.h"

#define SPRITESOURCELISTSIZE 100
//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
typedef struct SpriteSourceManager
{
	// The number of sprite sources currently in the list.
	unsigned int spriteSourceCount;

	// A list of all currently loaded sprite sources.
	// This list can be a fixed-length array (minimum size of 10 entries)
	// or a dynamically-sized array, such as a linked list.
	SpriteSourcePtr	spriteSourceList[SPRITESOURCELISTSIZE];

} SpriteSourceManager;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------
// 
// 
//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------
static SpriteSourceManager SpriteSourceManagerList;

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------
static void SpriteSourceMangerAdd(SpriteSourcePtr spriteSource);  // Add Sprite Source to List
static SpriteSourcePtr  SpriteSourceMangerFind(const char* name); // Find Sprite Source By name

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Initialize the Sprite Source Manager.
// (NOTE: Make sure to initialize all memory to zero.)
void SpriteSourceManagerInit()
{
	//initialize all memory to zero
	memset(SpriteSourceManagerList.spriteSourceList, 0, sizeof(SpriteSourceManagerList.spriteSourceList));
}

// Create a SpriteSource and add it to the SpriteSource manager.
// 1: Call SpriteSourceManagerFind() to determine if the sprite source already exists
// 2: If the named sprite source does not already exist,
//	  a: Use sprintf_s() to construct a path name using SpriteSourceName
//	     (HINT: The correct path name should be constructed using "Data/&s.txt".)
//	  b: Call StreamOpen(), passing the pathname
//    c: If the stream was opened successfully,
//		 1: Call SpriteSourceRead() to construct a SpriteSource using data read from the file
//		 2: Call SpriteSourceManagerAdd(), passing the created SpriteSource
// 3: Return the SpriteSource (already existing or just created)
// Params:
//	 SpriteSourceName = The name of the SpriteSource to be created.
// Returns:
//	 If the SpriteSource already exists or was created successfully,
//	   then return a pointer to the SpriteSource,
//	   else return NULL.
SpriteSourcePtr SpriteSourceManagerBuild(const char* spriteSourceName)
{
	if (spriteSourceName)
	{
		SpriteSourcePtr newSpriteSource = SpriteSourceMangerFind(spriteSourceName);

		if (newSpriteSource == NULL)
		{
			char pathName[32] = {0};

			sprintf_s(pathName, sizeof(pathName), "Data/%s.txt", spriteSourceName);

			Stream newStream = StreamOpen(pathName);

			if (newStream)
			{
				newSpriteSource = SpriteSourceRead(newStream);

				SpriteSourceMangerAdd(newSpriteSource);
			}
		}
		return newSpriteSource;
	}

	return NULL;	
}

// Free all SpriteSourcees in the SpriteSource Manager.
// (NOTE: Make sure to call SpriteSourceFree() to free each SpriteSource.)
// (HINT: The list should contain nothing but NULL pointers once this function is done.)
void SpriteSourceManagerFreeAll()
{  
	SpriteSourceManagerList.spriteSourceCount = 0;

	unsigned int i = 0; // Loop Var
	for (; i < SPRITESOURCELISTSIZE; i++) // Loop Through List
	{
		SpriteSourceManagerList.spriteSourceList[i] = NULL;
		SpriteSourceFree(&SpriteSourceManagerList.spriteSourceList[i]);
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
 
// This private function should accept a SpriteSourcePtr and 
 // insert the sprite source in the first available location in 
 // the sprite source list.
static void SpriteSourceMangerAdd(SpriteSourcePtr spriteSource)
{
	if (spriteSource != NULL) // If the pointer is valid 
	{
		// Increment OSprite Source Count
		SpriteSourceManagerList.spriteSourceCount++;

		unsigned int i = 0; // Loop Var
		for (; i < SPRITESOURCELISTSIZE; i++) // Loop Through List
		{
			if (!SpriteSourceManagerList.spriteSourceList[i]) // If Empty Space Found
			{
				break; // Break from Loop
			}
		}
		// Add Sprite source to List
		SpriteSourceManagerList.spriteSourceList[i] = spriteSource; 
	}
} // Add Sprite Source to List

// This private function should accept a “const char *” representing a name 
// and search through the sprite source list for a sprite source with a matching name.
// If a matching name is found, then this function should return a pointer to the 
// sprite source with the matching name
static SpriteSourcePtr SpriteSourceMangerFind(const char* name) 
{
	unsigned int i = 0; // Loop Var
	for (; i < SPRITESOURCELISTSIZE; i++) // Loop Through List
	{
		if (SpriteSourceIsNamed(SpriteSourceManagerList.spriteSourceList[i], name))
		  return SpriteSourceManagerList.spriteSourceList[i];
	}

	return NULL;
} // Find Sprite Source By name
