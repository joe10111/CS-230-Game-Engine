//------------------------------------------------------------------------------
//
// File Name:	Sprite.c
// Author(s):	Joe Centeno(joe.centeno)
// Project:		Project 3
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"       // Standard System Include Files
#include "Sprite.h"       // Texture 
#include "Trace.h"        // Trace Log System
#include "Stream.h"       // Stream System
#include "AEEngine.h"     // Alpha Engine
#include "Transform.h"    // PhysicsComponent 
#include "SpriteSource.h" // SpriteSource
#include "Vector2D.h"     // Vector Math
#include "Matrix2D.h"     // Matrix
#include "Mesh.h"         //Mesh
#include "MeshManager.h"
#include "SpriteSourceManager.h" 

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct Sprite
{
	// The frame currently being displayed (for sprite sheets).
	unsigned int frameIndex;

	// The alpha transparency to use when drawing the sprite.
	float alpha;

	// The sprite source used when drawing (NULL = simple colored mesh).
	SpriteSourcePtr spriteSource;

	// The mesh used to draw the sprite.
	AEGfxVertexList* mesh;

	const char* text;

} Sprite;


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


SpritePtr SpriteCreate(void)
{
	Sprite* tmpPtr = calloc(1, sizeof(Sprite));
	if (tmpPtr)
	{
		tmpPtr->alpha = 1.0f;
		return tmpPtr;
	}
	TraceMessage("Error: SpriteCreate Failed to Allocate Memory");
	return NULL;
}

//Clone Sprite 
SpritePtr SpriteClone(const SpritePtr other)
{
	if (other)
	{
		SpritePtr newSprite = SpriteCreate();
		*newSprite = *other;
		return newSprite;
	}

	TraceMessage("Error: SpriteClone requires a valid Pointer");
	return NULL;
}


void SpriteFree(SpritePtr* sprite)
{
	if (!sprite)
		return;
	free(*sprite);
	*sprite = NULL;
	return;
}

// Read a token that represents the name of a sprite source
// Call SpriteSourceManagerBuild(), passing the name of the sprite source
// Call SpriteSetSpriteSource(), passing the created sprite source

void SpriteRead(SpritePtr sprite, Stream stream)
{
	if(sprite && stream)
	{
		sprite->frameIndex = StreamReadInt(stream);
		sprite->alpha = StreamReadFloat(stream);

		const char* MeshName = StreamReadToken(stream);
		AEGfxVertexList* Mesh = MeshManagerBuild(MeshName);
		SpriteSetMesh(sprite, Mesh);

		const char* SpriteName = StreamReadToken(stream);
		SpriteSourcePtr newSpriteSrc = SpriteSourceManagerBuild(SpriteName);
		SpriteSetSpriteSource(sprite, newSpriteSrc);

		return;
	}
}

void SpriteDraw(const Sprite* sprite, TransformPtr transform)
{
	if (sprite && transform && sprite->mesh)
	{
		if (sprite->spriteSource)
		{
			//Vector2D tmpOffset = { 0, 0 }; Old Code

			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

			//SpriteSourceGetUV(sprite->spriteSource, sprite->frameIndex, &tmpOffset.x, &tmpOffset.y); Old Code

			//AEGfxTextureSet(SpriteSourceGetTexture(sprite->spriteSource), tmpOffset.x, tmpOffset.y); Old Code
		}
		else
		{
			AEGfxSetRenderMode(AE_GFX_RM_COLOR);
			AEGfxTextureSet(NULL, 0, 0);
		}

		AEGfxSetTransparency(sprite->alpha);
		AEGfxSetBlendColor(0, 0, 0, 0);

		if (sprite->text)
		{
			Matrix2D tmpMatrix = *TransformGetMatrix(transform);
			Matrix2D offset = {0};
			Matrix2DTranslate(&offset, TransformGetScale(transform)->x, 0.0f);
			const char* tmpText;
			tmpText = sprite->text;

			int i = 0;
			while (tmpText[i] != '\0')
			{
				int index = tmpText[i] - 32;
				MeshRender(sprite->mesh, sprite->spriteSource, &tmpMatrix, index);
				i++;
				Matrix2DConcat(&tmpMatrix, &offset, &tmpMatrix);
			}
			
		}
		else
		{
		  MeshRender(sprite->mesh, sprite->spriteSource, TransformGetMatrix(transform), sprite->frameIndex);
		}

	//Old Code
		//Vector2D tmpPos = *TransformGetTranslation(transform);
		//float tmpRotation = AERadToDeg(TransformGetRotation(transform));
		//Vector2D tmpScale = *TransformGetScale(transform);

		//AEGfxSetFullTransform(tmpPos.x, tmpPos.y, tmpRotation, tmpScale.x, tmpScale.y);
		//AEGfxSetTransparency(sprite->alpha);
		//AEGfxSetBlendColor(0.0f, 0.0f, 0.0f, 0.0f);
		//AEGfxMeshDraw(sprite->mesh, AE_GFX_MDM_TRIANGLES);
		return;
	}
	TraceMessage("Error: SpriteDraw Requires a valid Sprite and Transform");
	return;
}


float SpriteGetAlpha(SpritePtr sprite)
{
	if (sprite)
		return sprite->alpha;
	TraceMessage("Error: SpriteGetAlpha Requires a valid SpritePtr");
	return 0.0f;
}

void SpriteSetAlpha(SpritePtr sprite, float newAlpha)
{
	if (sprite)
		sprite->alpha = AEClamp(newAlpha, 0.0f, 1.0f);
	TraceMessage("Error: SpriteSetAlpha Requires a valid SpritePtr");
	return;
}


void SpriteSetFrame(SpritePtr sprite, unsigned int frameIndex)
{
	if (sprite)
	{
		sprite->frameIndex = frameIndex;
		TraceMessage("SpriteSetFrame: frame index = %d", frameIndex);
		return;
	}
	TraceMessage("Error: SpriteSetFrame Requires a valid SpritePtr");
	return;
}


void SpriteSetMesh(SpritePtr sprite, AEGfxVertexList* mesh)
{
	if (sprite && mesh)
	{
		sprite->mesh = mesh;
		return;
	}
	TraceMessage("Error: SpriteSetMesh Requires a valid SpritePtr and Mesh");
	return;
}

void SpriteSetSpriteSource(SpritePtr sprite, SpriteSourcePtr spriteSource)
{
	if (sprite != NULL && spriteSource != NULL)
	{
		sprite->spriteSource = spriteSource;
		return;
	}
	else if(sprite == NULL && spriteSource == NULL)
	{
		TraceMessage("Error: SpriteSetSpriteSource Requires a valid SpritePtr and SpriteSource");
		return;
	}
}

void SpriteSetText(SpritePtr sprite, const char* text)
{
	if (sprite)
	{
		sprite->text = text;
		return;
	}
	TraceMessage("Error: SpriteSetText Requires a valid Sprite");
	return;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
