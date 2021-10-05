//------------------------------------------------------------------------------
//
// File Name:	GameStateDemo.c
// Author(s):	Doug Schilling (dschilling), , Joe Centeno(joe.centeno)
// Project:		Project 2
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "AEEngine.h"
#include "GameStateManager.h"
#include "GameStateDemo.h"
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

	//------------------
	// DECLARE VARIABLES
	//------------------
    unsigned char colors[16];				// 2x2 image
    float obj1X = 0.0f, obj1Y = 0.0f;		// Position variables for object 1
    float obj1texX = 0, obj1texY = 0;		// Texture variables for object 2's texture
    AEGfxVertexList* pMesh1;				// Pointer to Mesh (Model)
    AEGfxVertexList* pMesh2;				// Pointer to Mesh (Model)
    AEGfxVertexList* pMeshLine;			// Pointer to Mesh (Model)
    AEGfxTexture* pTex1;					// Pointer to Texture (Image)
    AEGfxTexture* pTex2;					// Pointer to Texture (Image)
    float camX, camY;						// Used to temporary store camera position
    float alpha = 1.0f;
//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Load the resources associated with the Demo game state.
void GameStateDemoLoad()
{   //-------------
	//CREATE MESHES
	//-------------

	// 1 triangle at a time
	// X, Y, Color, texU, texV
	AEGfxTriAdd(
		-25.5f, -25.5f, 0xFFFF0000, 0.0f, 0.0f,
		25.5f, 0.0f, 0xFFFFff00, 0.0f, 0.0f,
		-25.5f, 25.5f, 0xFFFF0000, 0.0f, 0.0f);
	// Saving the QuadMesh (list of triangles) in pMesh1
	pMesh1 = AEGfxMeshEnd();
	AE_WARNING_MESG(pMesh1, "Failed to create mesh 1!!");

	// Informing the library that we're about to start adding triangles
	AEGfxMeshStart();
	// This shape has 2 triangles
	AEGfxTriAdd(
		-30.0f, -30.0f, 0x00FF00FF, 0.0f, 1.0f,
		30.0f, -30.0f, 0x00FFFF00, 1.0f, 1.0f,
		-30.0f, 30.0f, 0x00F00FFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		30.0f, -30.0f, 0x00FFFFFF, 1.0f, 1.0f,
		30.0f, 30.0f, 0x00FFFFFF, 1.0f, 0.0f,
		-30.0f, 30.0f, 0x00FFFFFF, 0.0f, 0.0f);
	// Saving the QuadMesh (list of triangles) in pMesh2
	pMesh2 = AEGfxMeshEnd();
	AE_WARNING_MESG(pMesh2, "Failed to create mesh 2!!");
	
	// Informing the library that we're about to start adding vertices
	AEGfxMeshStart();
	// This shape has 5 vertices
	AEGfxVertexAdd(0.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f);
	AEGfxVertexAdd(100.0f, 0.0f, 0xFFFFFFFF, 0.0f, 0.0f);
	AEGfxVertexAdd(200.0f, 150.0f, 0xFFFFFFFF, 0.0f, 0.0f);
	AEGfxVertexAdd(300.0f, -100.0f, 0xFFFFFFFF, 0.0f, 0.0f);
	AEGfxVertexAdd(100.0f, -250.0f, 0xFFFFFFFF, 0.0f, 0.0f);
	pMeshLine = AEGfxMeshEnd();
	AE_WARNING_MESG(pMeshLine, "Failed to create mesh 2!!");

	//---------------------
	// LOAD/CREATE TEXTURES
	//---------------------

	// Texture 1: From file
	pTex1 = AEGfxTextureLoad("./Assets/PlanetTexture.png");
	AE_WARNING_MESG(pTex1, "Failed to create texture1!!");

	// Texture 2: From memory
	// RGBA format
	colors[0] = 255;	colors[1] = 0;		colors[2] = 0;		colors[3] = 255;
	colors[4] = 0;		colors[5] = 255;	colors[6] = 0;		colors[7] = 255;
	colors[8] = 0;		colors[9] = 0;		colors[10] = 255;	colors[11] = 255;
	colors[12] = 255;	colors[13] = 255;	colors[14] = 255;	colors[15] = 255;
	pTex2 = AEGfxTextureLoadFromMemory(colors, 2, 2);
	AE_WARNING_MESG(pTex2, "Failed to create texture2!!");

}

// Initialize the memory associated with the Demo game state.
void GameStateDemoInit()
{   
	//------------------------------------
	// INIT VARS FOR RESET & START UP
	//------------------------------------
	obj1X = 0.0f;		// Position variables for object 1
	obj1Y = 0.0f;
	obj1texX = 0;		// Texture variables for object 2's texture
	obj1texY = 0;
	alpha = 1.0f;
	camX = 0.0f;
	camY = 0.0f;
	AEGfxSetCamPosition(camX, camY);

	//------------------------------------
	// SET BACKGROUND COLOR AND BLEND MODE
	//------------------------------------
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
}

// Update the Demo game state.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void GameStateDemoUpdate(float dt)
{
	/* Tell the compiler that the 'dt' variable is unused. */
	UNREFERENCED_PARAMETER(dt);

	//-----------------------------------------
	// UPDATE SPACESHIP POSITION & UPDATE ALPHA
	//-----------------------------------------

	// Object Control
	if (AEInputCheckCurr(VK_UP))
		obj1Y += 3.0f;
	else if (AEInputCheckCurr(VK_DOWN))
		obj1Y -= 3.0f;

	if (AEInputCheckCurr(VK_LEFT))
		obj1X -= 3.0f;
	else if (AEInputCheckCurr(VK_RIGHT))
		obj1X += 3.0f;

	// Alpha value
	if (AEInputCheckCurr('Z'))
		alpha -= 0.01f;
	else if (AEInputCheckCurr('X'))
		alpha += 0.01f;
	alpha = AEClamp(alpha, 0.0f, 1.0f);

	//-----------------------
	// UPDATE CAMERA POSITION
	//-----------------------

	// Move the camera
	AEGfxGetCamPosition(&camX, &camY);
	if (AEInputCheckCurr('W'))
		AEGfxSetCamPosition(camX, camY + 2);
	else if (AEInputCheckCurr('S'))
		AEGfxSetCamPosition(camX, camY - 2);

	AEGfxGetCamPosition(&camX, &camY);
	if (AEInputCheckCurr('A'))
		AEGfxSetCamPosition(camX - 2, camY);
	else if (AEInputCheckCurr('D'))
		AEGfxSetCamPosition(camX + 2, camY);

	//----------------------
	// UPDATE TEXTURE OFFSET
	//----------------------

	// Texture offset
	if (AEInputCheckCurr('L'))
		obj1texX += 0.01f;
	else if (AEInputCheckCurr('J'))
		obj1texX -= 0.01f;

	if (AEInputCheckCurr('I'))
		obj1texY += 0.01f;
	else if (AEInputCheckCurr('K'))
		obj1texY -= 0.01f;

	//----------------------------
	// DRAW LINE STRIP AND OBJECTS
	//----------------------------

	// Drawing line strip.
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetPosition(0.0f, 0.0f);
	AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
	AEGfxMeshDraw(pMeshLine, AE_GFX_MDM_LINES_STRIP);

	// Drawing object 1
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);			// This step must be done first.
	// Set position for object 1
	AEGfxSetPosition(obj1X, obj1Y);
	// No texture for object 1
	AEGfxTextureSet(NULL, 0, 0);
	// Drawing the QuadMesh (list of triangles)
	AEGfxSetTransparency(alpha);
	AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
	AEGfxMeshDraw(pMesh1, AE_GFX_MDM_TRIANGLES);	// This step must be done last.

	// Drawing object 2
	// Set position for object 2
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetPosition(100.0f, -60.0f);
	// Set texture for object 2
	AEGfxTextureSet(pTex1, obj1texX, obj1texY);
	AEGfxSetTransparency(1.0f);
	AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
	// Drawing the QuadMesh (list of triangles)
	AEGfxMeshDraw(pMesh2, AE_GFX_MDM_TRIANGLES);

	// Drawing object 2 again
	// Set position for object 2
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetPosition(-100.0f, -60.0f);
	// Set texture for object 2
	AEGfxTextureSet(pTex2, 0.0f, 0.0f);
	AEGfxSetTransparency(1.0f);
	AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
	// Drawing the QuadMesh (list of triangles)
	AEGfxMeshDraw(pMesh2, AE_GFX_MDM_TRIANGLES);

	// Drawing object 2 again and again
	// Set position for object 2
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetPosition(100.0f, 100.0f);
	// Set texture for object 2
	AEGfxTextureSet(pTex1, obj1texX, obj1texY);
	AEGfxSetTransparency(1.0f);
	AEGfxSetBlendColor(0.5f, 0.0f, 0.0, 0.75f);
	// Drawing the QuadMesh (list of triangles)
	AEGfxMeshDraw(pMesh2, AE_GFX_MDM_TRIANGLES);
		
	// Change GameState with Keys : 1 : 2 : 9 : 0
	if (AEInputCheckCurr('1'))
	{
		GameStateManagerSetNextState(GsLevel1);
	}

	if (AEInputCheckCurr('2'))
	{
		GameStateManagerSetNextState(GsLevel2);
	}

	if (AEInputCheckCurr('3'))
	{
		GameStateManagerSetNextState(GsAsteroids);
	}

	if (AEInputCheckCurr('4'))
	{
		GameStateManagerSetNextState(GsOmega);
	}

	if (AEInputCheckCurr('9'))
	{
		GameStateManagerSetNextState(GsSandbox);
	}

	if (AEInputCheckCurr('0'))
	{
		GameStateManagerSetNextState(GsRestart);
	}
}

// UnloadSprite any memory associated with the Demo game state.
void GameStateDemoShutdown()
{
}

// Unload the resources associated with the Demo game state.
void GameStateDemoUnload()
{
	//--------------------
	// UNLOAD ALL TEXTURES
	//--------------------

	// Freeing the objects and textures
	AEGfxMeshFree(pMesh1);
	AEGfxMeshFree(pMesh2);
	AEGfxMeshFree(pMeshLine);

	//----------------
	// FREE ALL MESHES
	//----------------

	AEGfxTextureUnload(pTex1);
	AEGfxTextureUnload(pTex2);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

