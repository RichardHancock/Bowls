#pragma once

#include <math.h>
#include "cgg/cgg.h"

/**
@brief Physics functions

A Namespace that contains functions for the physics in the game.
*/
namespace Physics
{
	bool ballHitBallCheck(float ball1Radius, float ball2Radius, cgg::Vec3 ball1Position, cgg::Vec3 ball2Position)
	{
		float radSum = ball1Radius + ball2Radius;
		float deltaX = ball1Position.x - ball2Position.x;
		float deltaY = ball1Position.y - ball2Position.y;
		float deltaZ = ball1Position.z - ball2Position.z;
		// sqrt (dX^2 + dY^2 + dZ^2)
		float distance = sqrtf((deltaX * deltaX) + (deltaY * deltaY) + (deltaZ * deltaZ));
		if (distance < radSum)
		{
			return true;
		}
		return false;
	}

	cgg::Vec3 newCollisionVector(cgg::Vec3 moveVectorBall1, cgg::Vec3 moveVectorBall2)
	{
		return (moveVectorBall1 + moveVectorBall2);
	}
};