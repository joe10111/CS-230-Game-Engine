//------------------------------------------------------------------------------
//
// File Name:	Matrix2D.c
// Author(s):	Joe Centeno(joe.centeno)
// Project:		Project 3
// Course:		CS230S21
//
// Copyright © 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Matrix2D.h"
#include "Vector2D.h"
#include "Trace.h"
#include <math.h>


//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

#define M_PI 3.1415926f

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

void Matrix2DIdentity(Matrix2D* pResult)
{
	if (pResult)
	{
		pResult->m[0][0] = 1; pResult->m[1][0] = 0; pResult->m[2][0] = 0;
		pResult->m[0][1] = 0; pResult->m[1][1] = 1; pResult->m[2][1] = 0;
		pResult->m[0][2] = 0; pResult->m[1][2] = 0; pResult->m[2][2] = 1;
		
		return;
	}
	TraceMessage("Error: Matrix2DIdentity requires a valid Matrix2D ptr");
	return;
}


void Matrix2DTranspose(Matrix2D* pResult, Matrix2D* pMtx)
{
	if (pResult && pMtx)
	{
		pResult->m[0][0] = pMtx->m[0][0]; pResult->m[1][0] = pMtx->m[0][1]; pResult->m[2][0] = pMtx->m[0][2];
		pResult->m[0][1] = pMtx->m[1][0]; pResult->m[1][1] = pMtx->m[1][1]; pResult->m[2][1] = pMtx->m[1][2];
		pResult->m[0][2] = pMtx->m[2][0]; pResult->m[1][2] = pMtx->m[2][1]; pResult->m[2][2] = pMtx->m[2][2];
		return;
	}
	TraceMessage("Error: Matrix2DTranspose requires two valid Matrix2D ptrs");
}

void Matrix2DConcat(Matrix2D* pResult, Matrix2D* pMtx0, Matrix2D* pMtx1)
{
	 // Temporary matrix.
	Matrix2D temp;

	 //Init Temp Matrix
	temp.m[0][0] = 0;
	temp.m[0][1] = 0;
	temp.m[0][2] = 0;

	if (pResult && pMtx0 && pMtx1) //If all Matrix passed through are valid 
	{
		 //Do Row 1 Math in Temp Matrix
		temp.m[0][0] = pMtx0->m[0][0] * pMtx1->m[0][0] + pMtx0->m[0][1] * pMtx1->m[1][0] + pMtx0->m[0][2] * pMtx1->m[2][0];
		temp.m[0][1] = pMtx0->m[0][0] * pMtx1->m[0][1] + pMtx0->m[0][1] * pMtx1->m[1][1] + pMtx0->m[0][2] * pMtx1->m[2][1];
		temp.m[0][2] = pMtx0->m[0][0] * pMtx1->m[0][2] + pMtx0->m[0][1] * pMtx1->m[1][2] + pMtx0->m[0][2] * pMtx1->m[2][2];

		 //Do Row 2 Math in Temp Matrix
		temp.m[1][0] = pMtx0->m[1][0] * pMtx1->m[0][0] + pMtx0->m[1][1] * pMtx1->m[1][0] + pMtx0->m[1][2] * pMtx1->m[2][0];
		temp.m[1][1] = pMtx0->m[1][0] * pMtx1->m[0][1] + pMtx0->m[1][1] * pMtx1->m[1][1] + pMtx0->m[1][2] * pMtx1->m[2][1];
		temp.m[1][2] = pMtx0->m[1][0] * pMtx1->m[0][2] + pMtx0->m[1][1] * pMtx1->m[1][2] + pMtx0->m[1][2] * pMtx1->m[2][2];

		 //Do row 3 Math in Temp Matrix
		temp.m[2][0] = pMtx0->m[2][0] * pMtx1->m[0][0] + pMtx0->m[2][1] * pMtx1->m[1][0] + pMtx0->m[2][2] * pMtx1->m[2][0];
		temp.m[2][1] = pMtx0->m[2][0] * pMtx1->m[0][1] + pMtx0->m[2][1] * pMtx1->m[1][1] + pMtx0->m[2][2] * pMtx1->m[2][1];
		temp.m[2][2] = pMtx0->m[2][0] * pMtx1->m[0][2] + pMtx0->m[2][1] * pMtx1->m[1][2] + pMtx0->m[2][2] * pMtx1->m[2][2];

		 //Copie math row 1 results from temp to pResult
		pResult->m[0][0] = temp.m[0][0];
		pResult->m[0][1] = temp.m[0][1];
		pResult->m[0][2] = temp.m[0][2];
		 
		 //Copie math row 1 results from temp to pResult
		pResult->m[1][0] = temp.m[1][0];
		pResult->m[1][1] = temp.m[1][1];
		pResult->m[1][2] = temp.m[1][2];

		 //Copie math row 1 results from temp to pResult
		pResult->m[2][0] = temp.m[2][0];
		pResult->m[2][1] = temp.m[2][1];
		pResult->m[2][2] = temp.m[2][2];

		 //Assigns adress of pResults to Temp
		pResult = &temp;
	
		return;
	}
	TraceMessage("Error: Matrix2DConcat requires 3 valid Matrix2D ptrs");
	return;
}


void Matrix2DTranslate(Matrix2D* pResult, float x, float y)
{
	if (pResult)
	{
		pResult->m[0][0] = 1.0f; pResult->m[0][1] = 0.0f; pResult->m[0][2] = x;
		pResult->m[1][0] = 0.0f; pResult->m[1][1] = 1.0f; pResult->m[1][2] = y;
		pResult->m[2][0] = 0.0f; pResult->m[2][1] = 0.0f; pResult->m[2][2] = 1.0f;
		return;
	}
	TraceMessage("Error: Matrix2DTranslate requires a valid Matrix2D ptr");
	return;
}


void Matrix2DScale(Matrix2D* pResult, float x, float y)
{
	if (pResult)
	{
		pResult->m[0][0] = x;	 pResult->m[0][1] = 0.0f; pResult->m[0][2] = 0.0f;
		pResult->m[1][0] = 0.0f; pResult->m[1][1] = y;	  pResult->m[1][2] = 0.0f;
		pResult->m[2][0] = 0.0f; pResult->m[2][1] = 0.0f; pResult->m[2][2] = 1.0f;
		return;
	}
	TraceMessage("Error: Matrix2DScale requires a valid Matrix2D ptr");
}

void Matrix2DRotDeg(Matrix2D* pResult, float angle)
{
	Matrix2DIdentity(pResult);

	double angle_convert = angle * (M_PI / 180);

	double cos_angle = cos(angle_convert);
	double sin_angle = sin(angle_convert);

	pResult->m[0][0] = (float)cos_angle;
	pResult->m[0][1] = (float)-sin_angle;
	pResult->m[1][0] = (float)sin_angle;
	pResult->m[1][1] = (float)cos_angle;

}

void Matrix2DRotRad(Matrix2D* pResult, float angle)
{
	if (pResult)
	{
		pResult->m[0][0] = cosf(angle);  pResult->m[0][1] = -sinf(angle); pResult->m[0][2] = 0.0f;
		pResult->m[1][0] = sinf(angle);  pResult->m[1][1] = cosf(angle);  pResult->m[1][2] = 0.0f;
		pResult->m[2][0] = 0.0f;		 pResult->m[2][1] = 0.0f;		  pResult->m[2][2] = 1.0f;
		return;
	}
	TraceMessage("Error: Matrix2DRotRad requires a valid Matrix2d ptr");
	return;
}


void Matrix2DMultVec(Vector2D* pResult, Matrix2D* pMtx, Vector2D* pVec)
{
	if (pResult && pMtx && pVec)
	{
		static Vector2D TempVector = {0, 0};

		// Set the x
		TempVector.x = pVec->x * pMtx->m[0][0] + pVec->y * pMtx->m[0][1] + pMtx->m[0][2];

		// Set the y
		TempVector.y = pVec->x * pMtx->m[1][0] + pVec->y * pMtx->m[1][1] + pMtx->m[1][2];

		// Copy the temp to pResult.
		*pResult = TempVector;
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
