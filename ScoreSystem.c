//------------------------------------------------------------------------------
//
// File Name:	ScoreSystem.c
// Author(s):	Joe Centeno (joe.centeno)
// Project:		Project 6
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "ScoreSystem.h"
#include "Vector2D.h"
#include "GameObject.h"
#include "BehaviorHudText.h"
#include "GameObjectManager.h"

#define ZERO 0
//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------
static int asteroidScore = 0;
static int asteroidHighScore = 0;
static int asteroidWaveCount;

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------
// Initialize the Score system.
// (NOTE: All private variables must be initialized to 0.)
// (NOTE: There should be separate values for Score, High Score, and Wave.)
void ScoreSystemInit()
{
	asteroidScore = ZERO;
	asteroidHighScore = ZERO;
	asteroidWaveCount = ZERO;
}

// Restart the Score system after updating the high score.
// (HINT: The high score value should be updated before zeroing out the score and wave variables.
void ScoreSystemRestart()
{
	// Update high score.
	if (asteroidScore > asteroidHighScore)
	{
		asteroidHighScore = asteroidScore;
	}

	asteroidScore = ZERO;
	asteroidWaveCount = ZERO;
}

// Get the specified score system value.
// (Hint: Check to make sure that ssid is between SsiInvalid and SsiMax (non-inclusive).)
// Params:
//	 ssid = The score system value to be returned.
// Returns:
//	 If "ssid" is a valid index,
//	   then return the value associated with that index,
//	   else return 0.
int ScoreSystemGetValue(ScoreSystemId ssid)
{
	if (ssid > SsiInvalid && ssid < SsiMax)
	{
		switch (ssid)
		{
			// Score 
		case SsiScore:
			return asteroidScore;
			break;
			// High Score
		case SsiHighScore:
			return asteroidHighScore;
			break;
			// Wave Count
		case SsiWaveCount:
			return asteroidWaveCount;
			// Invalid
		case SsiInvalid:
			return SsiInvalid;
		default:
			return ZERO;
			break;
		}
	}
	return ZERO;
}

// Increase the game score by score value.
// Params:
//	 scoreValue = The amount to be added to the game score.
void ScoreSystemIncreaseScore(unsigned int scoreValue)
{
	asteroidScore += scoreValue;
}

// Increase the wave count by 1.
void ScoreSystemIncreaseWaveCount()
{
	asteroidWaveCount++;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

