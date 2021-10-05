//------------------------------------------------------------------------------
//
// File Name:	Mesh.c
// Author(s):	Joe Centeno(joe.centeno)
// Project:		Project 2
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Mesh.h"
#include "AEEngine.h"
#include "Vector2D.h"
#include "Trace.h"
#include "Stream.h"
#include "SpriteSource.h"
#include "Matrix2D.h"

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


AEGfxVertexList* MeshCreateQuad(float xHalfSize, float yHalfSize, float uSize, float vSize, const char* Label)
{
	AEGfxVertexList* tmpMesh;

	// Informing the library that we are about to start adding triangles
	AEGfxMeshStart();

	// This shape has 2 triangles
	AEGfxTriAdd(
		-xHalfSize, -yHalfSize, 0xFFFFFFFF, 0.0f, vSize,
		xHalfSize, -yHalfSize, 0xFFFFFFFF, uSize, vSize,
		-xHalfSize, yHalfSize, 0xFFFFFFFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		xHalfSize, -yHalfSize, 0xFFFFFFFF, uSize, vSize,
		xHalfSize, yHalfSize, 0xFFFFFFFF, uSize, 0.0f,
		-xHalfSize, yHalfSize, 0xFFFFFFFF, 0.0f, 0.0f);

	tmpMesh = AEGfxMeshEnd();
	AE_ASSERT_MESG(tmpMesh, "Failed to create mesh %s!!", &Label);

	if (tmpMesh)
		return tmpMesh;

	TraceMessage("Error: MeshCreateQuad Could not create mesh");

	return NULL;
}

AEGfxVertexList* MeshRead(Stream stream)
{
	if (stream)
	{
		const char* token = StreamReadToken(stream);

		if (strstr(token, "Mesh"))
		{
			AEGfxMeshStart();

			int vertexCount = StreamReadInt(stream);

			for (int i = 0; i < vertexCount; i++)
			{
				Vector2D vertexPos = { StreamReadFloat(stream), StreamReadFloat(stream) };
				unsigned vertexColor = StreamReadHex(stream);
				Vector2D vertexUV = { StreamReadFloat(stream), StreamReadFloat(stream) };
				AEGfxVertexAdd(vertexPos.x, vertexPos.y, vertexColor, vertexUV.x, vertexUV.y);
			}
			return AEGfxMeshEnd();
		}
		else if (strstr(token, "Quad"))
		{

		    Vector2D halfSize;
		    StreamReadVector2D(stream, &halfSize);
		   
			int col = StreamReadInt(stream);
			int row = StreamReadInt(stream);

			const char* quadName = StreamReadToken(stream);
			
			AEGfxVertexList* tempQuad = MeshCreateQuad(halfSize.x, halfSize.y, 1.0f / (float)col, 1.0f / (float)row, quadName);
			return tempQuad;
		}
	}

	TraceMessage("Error: MeshRead requires a valid stream");

	return NULL;
}

void MeshRender(AEGfxVertexList* mesh, SpriteSource* spriteSource, Matrix2D* transform, unsigned frameIndex)
{
	if (mesh && transform)
	{
		if (spriteSource)
		{
			float u, v = 0;

			float* u2 = &u;
			float* v2 = &v;

			SpriteSourceGetUV(spriteSource, frameIndex, u2, v2);
			AEGfxTextureSet(SpriteSourceGetTexture(spriteSource), u, v);
		}

		AEGfxSetTransform(transform->m);
		AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);

		return;
	}

	TraceMessage("Error: MeshRender Could not render");

	return;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------