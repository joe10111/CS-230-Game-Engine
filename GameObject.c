//------------------------------------------------------------------------------
//
// File Name:	GameObject.c
// Author(s):	Joe Centeno(joe.centeno)
// Project:		Project 2
// Course:		CS230SU21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameObject.h"
#include "Trace.h"
#include "Physics.h"
#include "Stream.h"
#include "Sprite.h"
#include "Collider.h"
#include "ColliderCircle.h"
#include "ColliderLine.h"
#include "Transform.h"
#include "Physics.h"
#include "Animation.h"
#include "Behavior.h"
#include "BehaviorBullet.h"
#include "BehaviorSpaceship.h"
#include "GameObjectManager.h"
#include "BehaviorHudText.h"
#include "BehaviorAsteroid.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct GameObject
{
	char name[32]; // -- Name of Game Object

	AnimationPtr animation; // -- Pointer to an animation component

	BehaviorPtr behavior; // -- Pointer to an behavior component

	PhysicsPtr physics; // -- Pointer to an animation component
	
	SpritePtr sprite; // -- Pointer to an sprite component

	TransformPtr transform; // -- Pointer to an transform component

	ColliderPtr collider; // -- Pointer to an Colider component 

	bool isDestroyed; // -- GameObject Destroy Bool 

} GameObject;

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

GameObjectPtr GameObjectCreate(void)
{
	GameObject* tmpGameObj = calloc(1, sizeof(GameObject));
	if (tmpGameObj)
		return tmpGameObj;
	TraceMessage("Error: GameObjectCreate Could not allocate memory properly");
	return NULL;
}

//Frees the game object 
// Uses Frees the game object componets  
// -- Sprite
// -- Animation 
// -- Physics
// -- Transform

void GameObjectFree(GameObjectPtr* gameObject)
{
	if (gameObject)
	{
		SpritePtr sprite = GameObjectGetSprite(*gameObject);          // <-- Sprite Pointer
		AnimationPtr animation = GameObjectGetAnimation(*gameObject); // <-- Animation Pointer
		PhysicsPtr physics = GameObjectGetPhysics(*gameObject);       // <-- Physics Pointer
		TransformPtr transform = GameObjectGetTransform(*gameObject); // <-- Transform Pointer
		BehaviorPtr behvior = GameObjectGetBehavior(*gameObject);     // <-- Behavior Pointer
		ColliderPtr collider = GameObjectGetCollider(*gameObject);    // <-- Collider Pointer

		SpriteFree(&sprite);       // x Sprite Freed
		AnimationFree(&animation); // x Animation Freed
		PhysicsFree(&physics);     // x Physics Freed
		TransformFree(&transform); // x Transform Freed
		BehaviorFree(&behvior);    // x Behvior Freed
		ColliderFree(&collider);   // x Colider Freed

		free(*gameObject);  // <-- Free GameObject 
		*gameObject = NULL; // <-- Set Pointer to Null

		return;
	}
	else
	{
		TraceMessage("Requires Valid GameObject");
	}
	
}

void GameObjectRead(GameObjectPtr gameObject, Stream stream)
{
	if (gameObject && stream)
	{
		const char* tmpToken = StreamReadToken(stream);
		GameObjectSetName(gameObject, tmpToken);

		while (strlen(tmpToken) != 0 || *tmpToken == '\0')
		{
			tmpToken = StreamReadToken(stream);

			if (strstr(tmpToken, "Transform"))
			{
				gameObject->transform = TransformCreate();
				TransformRead(gameObject->transform, stream);
				GameObjectAddTransform(gameObject, gameObject->transform);
				
			}
			else if (strstr(tmpToken, "Physics"))
			{
				gameObject->physics = PhysicsCreate();
				PhysicsRead(gameObject->physics, stream);
				GameObjectAddPhysics(gameObject, gameObject->physics);
			}
			else if (strstr(tmpToken, "Sprite"))
			{
				gameObject->sprite = SpriteCreate();
				SpriteRead(gameObject->sprite, stream);
				GameObjectAddSprite(gameObject, gameObject->sprite);
			}
			else if (strstr(tmpToken, "Animation"))
			{
				AnimationPtr anim = AnimationCreate();
				AnimationRead(anim, stream);
				GameObjectAddAnimation(gameObject, anim);
			}
			else if (strstr(tmpToken, "BehaviorSpaceship"))
			{
				BehaviorPtr tmpBeh = BehaviorSpaceshipCreate();
				BehaviorRead(tmpBeh, stream);
				GameObjectAddBehavior(gameObject, tmpBeh);
			}
			else if (strstr(tmpToken, "BehaviorBullet"))
			{
				BehaviorPtr tmpBeh = BehaviorBulletCreate();
				BehaviorRead(tmpBeh, stream);
				GameObjectAddBehavior(gameObject, tmpBeh);
			}
			else if (strstr(tmpToken, "BehaviorHudText"))
			{
				BehaviorPtr tmpBeh = BehaviorHudTextCreate();
				BehaviorHudTextRead(tmpBeh, stream);
				GameObjectAddBehavior(gameObject, tmpBeh);
			}
			else if (strstr(tmpToken, "BehaviorAsteroid"))
			{
				BehaviorPtr tmpBeh = BehaviorAsteroidCreate();
				BehaviorRead(tmpBeh, stream);
				GameObjectAddBehavior(gameObject, tmpBeh);
			}
			else if (strstr(tmpToken, "ColliderCircle"))
			{
				ColliderPtr tmpColl = ColliderCircleCreate();
				ColliderCircleRead(tmpColl, stream);
				GameObjectAddCollider(gameObject, tmpColl);
			}
			else if (strstr(tmpToken, "ColliderLine"))
			{ 
				ColliderPtr tmpColl = ColliderLineCreate();
				ColliderLineRead(tmpColl, stream);
				GameObjectAddCollider(gameObject, tmpColl);
			}
			else
			{
				break;
			}
		}
		return;
	}

	TraceMessage("Error: GameObjectRead Requires a valid Game Object and Stream");
	return;
}

void GameObjectAddAnimation(GameObjectPtr gameObject, AnimationPtr animation)
{
	if (gameObject && animation)
	{
		AnimationSetParent(animation, gameObject);
		gameObject->animation = animation;
		return;
	}
	else if(gameObject == NULL && animation == NULL)
	{
		TraceMessage("Error: GameObjectAddAnimation requires a valid Animation ptr and a valid GameObject pointer");
		return;
	}
}

void GameObjectAddBehavior(GameObjectPtr gameObject, BehaviorPtr behavior)
{
	if (gameObject && behavior)
	{
		gameObject->behavior = behavior;
		BehaviorSetParent(behavior, gameObject);

		return;
	}

	TraceMessage("Error: GameObjectAddBehavior requires a valid game object ptr and a valid Behavior ptr");
	return;
}


void GameObjectAddPhysics(GameObjectPtr gameObject, PhysicsPtr physics)
{
	if (gameObject && physics)
	{
		gameObject->physics = physics;
		return;
	}
	//TraceMessage("Error: GameObjectAddPhysics Requires two valid pointers");
	return;
}

void GameObjectAddSprite(GameObjectPtr gameObject, SpritePtr sprite)
{
	if (gameObject && sprite)
	{
		gameObject->sprite = sprite;
		return;
	}
	TraceMessage("Error: GameObjectAddSprite Requires two valid pointers");
	return;
}

void GameObjectAddTransform(GameObjectPtr gameObject, TransformPtr transform)
{
	if (gameObject && transform)
	{
		gameObject->transform = transform;
		return;
	}
	TraceMessage("Error: GameObjectAddTransform Requires two valid pointers");
	return;
}

void GameObjectAddCollider(GameObjectPtr gameObject, ColliderPtr colider)
{
	if (gameObject && colider)
	{
		gameObject->collider = colider;
		ColliderSetParent(colider, gameObject);

		return;
	}
	
	//TraceMessage("Error : Requires Valid Pointers");
	return;
}

void GameObjectSetName(GameObjectPtr gameObject, const char* name)
{
	if (gameObject)
	{
		// strncpy_s(gameObject->name, _countof(gameObject->name), name, _countof(name));
		strcpy_s(gameObject->name, _countof(gameObject->name), name);
		return;
	}
	TraceMessage("Error: GameObjectSetName Requires two valid pointers");
	return;
}

//Get Object Clone
GameObjectPtr GameObjectClone(const GameObjectPtr other)
{
	if (other)
	{
		GameObjectPtr newObj = GameObjectCreate();

		if (newObj)
		{
			//newObj->transform = TransformClone(other->transform);
			GameObjectAddTransform(newObj, TransformClone(other->transform));

			//newObj->animation = AnimationClone(other->animation);
			GameObjectAddAnimation(newObj, AnimationClone(other->animation));

			//newObj->behavior = BehaviorClone(other->behavior);
			GameObjectAddBehavior(newObj, BehaviorClone(other->behavior));

			//newObj->physics = PhysicsClone(other->physics);
			GameObjectAddPhysics(newObj, PhysicsClone(other->physics));

			//newObj->sprite = SpriteClone(other->sprite);
			GameObjectAddSprite(newObj, SpriteClone(other->sprite));

			GameObjectAddCollider(newObj, ColliderClone(other->collider));

			GameObjectSetName(newObj, other->name);

			return newObj;
		}
	}

	TraceMessage("Error: GameObjectClone requires a valid game object pointer");
	return NULL;
}

AnimationPtr GameObjectGetAnimation(const GameObjectPtr gameObject)
{
	if (gameObject)
	{
		return gameObject->animation;
	}
	TraceMessage("Error: GameObjectGetAnimation requires a valid gameobject");
	return NULL;
}

const char* GameObjectGetName(const GameObject* gameObject)
{
	if (gameObject)
	{
		return gameObject->name;
	}
		
	TraceMessage("Error: GameObjectGetName Requires a valid gameobject");
	return NULL;
}

BehaviorPtr GameObjectGetBehavior(const GameObjectPtr gameObject)
{
	if (!gameObject)
	{
		TraceMessage("Error: GameObjectGetBehavior requires a valid gameobject");
		return NULL;
	}

	return gameObject->behavior;
}

PhysicsPtr GameObjectGetPhysics(const GameObjectPtr gameObject)
{
	if (gameObject)
		return gameObject->physics;
	TraceMessage("Error: GameObjectGetPhysics Requires a valid gameObject");
	return NULL;
}


SpritePtr GameObjectGetSprite(const GameObjectPtr gameObject)
{
	if (gameObject)
		return gameObject->sprite;
	TraceMessage("Error: GameObjectGetSprite Requires a valid gameObject");
	return NULL;
}


TransformPtr GameObjectGetTransform(const GameObjectPtr gameObject)
{
	if (gameObject)
		return gameObject->transform;
	TraceMessage("Error: GameObjectGetTransform Requires a valid gameObject");
	return NULL;
}

ColliderPtr GameObjectGetCollider(const GameObjectPtr gameObject)
{
	if (gameObject)
		return gameObject->collider;

	TraceMessage("Error : Requires Valid Pointer");
	return NULL;
}

void GameObjectUpdate(GameObjectPtr gameObject, float dt)
{
	if (gameObject)
	{
		if (gameObject->animation)
		{
			AnimationUpdate(gameObject->animation, dt);
		}

		if (gameObject->behavior)
		{
			BehaviorUpdate(gameObject->behavior, dt);
		}

		if (gameObject->physics)
		{
			PhysicsUpdate(gameObject->physics, gameObject->transform, dt);
		}
		return;
	}
	else if (gameObject == NULL)
	{
		//TraceMessage("Error: GameObjectUpdate Requires a valid GameOjbect");
		return;
	}
}

void GameObjectDraw(GameObjectPtr gameObject)
{
	if (gameObject)
	{
		SpriteDraw(gameObject->sprite, gameObject->transform);
		return;
	}
	else if (gameObject == NULL)
	{
		//TraceMessage("Error: GameObjectDraw Requires a valid GameOjbect");
		return;
	}
}

void GameObjectDestroy(GameObjectPtr gameObject)
{
	if (!gameObject)
	{
		TraceMessage("Error: GameObjectDestroy requires a valid game object pointer");
		return;
	}

	gameObject->isDestroyed = true;
	return;
}

bool GameObjectIsDestroyed(GameObjectPtr gameObject)
{
	if (gameObject)
	{
		if (gameObject->isDestroyed)
			return true;
	    return false;
	}
	else if (!gameObject)
	{
		//TraceMessage("Error: GameObjectIsDestroyed requires a valid game object pointer");
		return false;
	}

	return false;
}

bool GameObjectIsNamed(const GameObject* gameObject, const char* name)
{
	if (!gameObject)
	{
		return false;
	}

	if (strcmp(gameObject->name, name) == 0)
		return true;
	return false;

}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
