//------------------------------------------------------------------------------
//
// File Name:	SpriteSource.c
// Author(s):	Joe Centeno(joe.centeno)
// Project:		Project 2
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "SpriteSource.h"
#include "SpriteSourceManager.h"
#include "Trace.h"
#include "Stream.h"
#include "MeshManager.h"
#include <AEEngine.h>

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
typedef struct SpriteSource
{
	// The name of the sprite source.
	// A buffer is used to allow each sprite source to have a unique name.
	// The buffer is hardcoded to an arbitrary length that will be sufficient
	//	 for all CS230 assignments.  You may, instead, use dynamically-allocated
	//	 arrays for this purpose but the additional complexity is unnecessary
	//	 and it is your responsibility to ensure that the memory is successfully
	//	 allocated and deallocated in all possible situations.
	char name[32];

	// The dimensions of the sprite sheet.
	int	numRows;
	int	numCols;

	// Pointer to a texture created using the Alpha Engine.
	AEGfxTexturePtr	pTexture;

} SpriteSource;

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

SpriteSourcePtr SpriteSourceCreate(int numCols, int numRows, AEGfxTexturePtr pTexture)
{
	SpriteSourcePtr spriteSourceTmp = (SpriteSourcePtr)calloc(1, sizeof(SpriteSource));
	if (spriteSourceTmp)
	{
		spriteSourceTmp->numCols = numCols;
		spriteSourceTmp->numRows = numRows;
		spriteSourceTmp->pTexture = pTexture;
		return spriteSourceTmp;
	}
	TraceMessage("Error: SpriteSourceCreate Could not allocate memory for Sprite Source.");
	return NULL;
}

void SpriteSourceFree(SpriteSourcePtr * spriteSource)
{
	if (spriteSource == NULL)
		return;
	AEGfxTexture* texture = SpriteSourceGetTexture(*spriteSource);
	AEGfxTextureUnload(texture);

	free(*spriteSource);
	*spriteSource = NULL;
}


AEGfxTexturePtr SpriteSourceGetTexture(SpriteSourcePtr spriteSource)
{
	if (spriteSource)
		return spriteSource->pTexture;
	TraceMessage("Error: SpriteSourceGetTexture Requires a valid SpriteSource");
	return NULL;
}

unsigned int SpriteSourceGetFrameCount(SpriteSourcePtr spriteSource)
{
	if (spriteSource)
		return spriteSource->numCols * spriteSource->numRows;
	TraceMessage("Error: SpriteSourceGetFrameCount Requires a valid SpriteSource");
	return 0;
}

void SpriteSourceGetUV(SpriteSourcePtr spriteSource, unsigned int frameIndex, float* u, float* v)
{
	if (spriteSource)
	{
		float uSize = 1.0f / spriteSource->numCols;
		float vSize = 1.0f / spriteSource->numRows;

		*u = uSize * (frameIndex % spriteSource->numCols);
		*v = vSize * (frameIndex / spriteSource->numCols);

		return;
	}
	TraceMessage("Error: SpriteSOurceGetUV Requires a valid SpriteSource");
	return;
}

// Create a SpriteSource using data read from a file.
// Params:
//	 stream = Pointer to the data stream used for reading.
// Returns:
//	 If the sprite source was created successfully,
//	   then return a pointer to the created sprite source,
//	   else return NULL.
SpriteSourcePtr SpriteSourceRead(Stream stream)
{
	// SpriteSourceRead should create and read in the values
	// of the sprite source, returning the created sprite source.

	if (stream)
	{				
	  const char* SpriteSrc;
	  const char* SrcName;
	  const char* srcPath;

	  int coll = 0;
	  int row = 0;

	  AEGfxTexturePtr newTexture = NULL;

	  SpriteSourcePtr newSpriteSource = SpriteSourceCreate(coll, row, newTexture);

	  SpriteSrc = StreamReadToken(stream);

	  if (strstr(SpriteSrc, "SpriteSource"))
	  {
		  SrcName = StreamReadToken(stream);

		  if (SpriteSourceIsNamed(newSpriteSource, SrcName) == 0)
		  {
			  strcpy_s(newSpriteSource->name, sizeof(newSpriteSource->name), SrcName);
		  }

		  coll = StreamReadInt(stream);
		  newSpriteSource->numCols = coll;

		  row = StreamReadInt(stream);
		  newSpriteSource->numRows = row;

		  srcPath = StreamReadToken(stream);
		  newTexture = AEGfxTextureLoad(srcPath);
		  newSpriteSource->pTexture = newTexture;
		  
	     return newSpriteSource;
	  }
	}

	return NULL;
}

// Determines if a sprite source has the specified name.
// Params:
//	 spriteSource = Pointer to the sprite source object.
//	 name = Pointer to the name to be compared.
// Returns:
//	 If the sprite source and name pointers are valid,
//		then perform a string comparison and return the result (match = true),
//		else return false.
bool SpriteSourceIsNamed(SpriteSourcePtr spriteSource, const char* name)
{
	if (!spriteSource)
	{
		return false;
	}

	if (strstr(spriteSource->name, name))
		return true;
	return false;
}

//------------------------------------------------------------------------------
// Private Functions:
//-----------------------------------------------------------------------------------