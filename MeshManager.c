//------------------------------------------------------------------------------
//
// File Name:	MeshManager.c
// Author(s):	Joe Centeno
// Project:		Project 4
// Course:		CS230Su21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "MeshManager.h"
#include "Mesh.h"
#include "Trace.h"
#include "Stream.h"
#include "AEEngine.h"

#define MESH_LIST_SIZE 256

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
typedef struct MeshManager
{
	// The number of game objects currently in the list.
	unsigned int meshCount;

	// The maximum number of game objects ever in the list, at any given moment.
	//unsigned int meshMax;

	// A list of all currently loaded meshes.
	// This list can be a fixed-length array (minimum size of 10 entries)
	// or a dynamically-sized array, such as a linked list.
	AEGfxVertexList* meshList[MESH_LIST_SIZE];

} MeshManager;

static MeshManager tmpMeshManager;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

void MeshManagerAdd(AEGfxVertexList* mesh);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Initialize the Mesh Manager.
// (NOTE: Make sure to initialize all memory to zero.)
void MeshManagerInit()
{
	memset(tmpMeshManager.meshList, 0, MESH_LIST_SIZE * sizeof(tmpMeshManager.meshList[0]));
}

// Create a mesh and add it to the mesh manager.
// 1: Use sprintf_s() to construct a path name using meshName
//	   (HINT: The correct path name should be constructed using "Data/&s.txt".)
// 2: Call StreamOpen(), passing the pathname
// 3: If the stream was opened successfully,
//	  a: Call MeshRead() to construct a mesh using data read from the file
//	  b: Call MeshManagerAdd(), passing the created mesh
//	  c: Close the stream
//	  d: Return the created mesh
// Params:
//	 meshName = The name of the mesh to be created.
// Returns:
//	 If the mesh was created successfully,
//	   then return a pointer to the created mesh,
//	   else return NULL.
AEGfxVertexList* MeshManagerBuild(const char* meshName)
{
	char pathName[FILENAME_MAX] = "";
	sprintf_s(pathName, _countof(pathName), "Data/%s.txt", meshName);
	Stream stream = StreamOpen(pathName);
	if (stream)
	{
		AEGfxVertexList* mesh = MeshRead(stream);
		if (!mesh)
		{
			TraceMessage("Error: MeshManagerBuild could not add the new mesh to the Mesh Manger");
			return NULL;
		}
		MeshManagerAdd(mesh);

		StreamClose(&stream);
		return mesh;
	}
	TraceMessage("Error: MeshManagerBuild could not open a filestream");
	return NULL;
}

void MeshManagerAdd(AEGfxVertexList* mesh)
{
	tmpMeshManager.meshList[tmpMeshManager.meshCount] = mesh;
	tmpMeshManager.meshCount++;

	// if (!mesh)
	// {
	// 	TraceMessage("Error: MeshManagerAdd requires a valid vertex list ptr");
	// 	return false;
	// }
	// int max = MESH_LIST_SIZE;
	// for (int i = 0; i < max; i++)
	// {
	// 	if (!tmpMeshManager.meshList[i])
	// 	{
	// 		tmpMeshManager.meshList[i] = mesh;
	// 		return true;
	// 	}
	// }
	// TraceMessage("Error: No open spots in the MeshManager");
	// return false;
}

// Free all meshes in the Mesh Manager.
// (NOTE: Make sure to call AEGfxMeshFree() to free each mesh.)
// (HINT: The list should contain nothing but NULL pointers once this function is done.)
void MeshManagerFreeAll()
{
	for (int i = 0; i < MESH_LIST_SIZE; i++)
	{
		if (tmpMeshManager.meshList[i])
		{
			AEGfxMeshFree(tmpMeshManager.meshList[i]);
			tmpMeshManager.meshList[i] = NULL;
		}
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
