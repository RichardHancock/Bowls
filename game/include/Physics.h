#pragma once

#include <math.h>
#include "cgg/cgg.h"
#include "Bowl.h"

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

	void newCollisionVelocities(Bowl* ball1, Bowl* ball2)
	{
		if (ball1->getXVelocity() == 0)
		{
			ball1->updateXVelocity((ball2->getXVelocity()));
			ball2->updateXVelocity((ball2->getXVelocity()*0.5f));
		}
		if (ball1->getZVelocity() == 0)
		{
			ball1->updateZVelocity((ball2->getZVelocity()));
			ball2->updateZVelocity((ball2->getZVelocity()*0.5f));
		}

		if (ball2->getXVelocity() == 0)
		{
			ball2->updateXVelocity((ball1->getXVelocity()));
			ball1->updateXVelocity((ball1->getXVelocity()*0.5f));
		}
		if (ball2->getZVelocity() == 0)
		{
			ball2->updateZVelocity((ball1->getZVelocity()));
			ball1->updateZVelocity((ball1->getZVelocity()*0.5f));
		}

		float tmpRedX = ball1->getXVelocity();
		float tmpRedZ = ball1->getZVelocity();

		ball1->updateXVelocity((ball2->getXVelocity()*0.5f));
		ball1->updateZVelocity((ball2->getZVelocity()*0.5f));
		ball2->updateXVelocity((tmpRedX*0.5f));
		ball2->updateZVelocity((tmpRedZ*0.5f));
	}

	void applyFriction(Bowl* ball)
	{
		// x friction
		if (ball->getXVelocity() < 0)
		{
			ball->updateXVelocity(ball->getXVelocity() + 0.1);
		}
		if (ball->getXVelocity() > 0)
		{
			ball->updateXVelocity(ball->getXVelocity() - 0.1);
		}
		if (ball->getXVelocity() < 0.1 && ball->getXVelocity() > -0.1)
		{
			ball->updateXVelocity(0.0f);
		}

		// z friction
		if (ball->getZVelocity() < 0)
		{
			ball->updateZVelocity(ball->getZVelocity() + 0.1);
		}
		if (ball->getZVelocity() > 0)
		{
			ball->updateZVelocity(ball->getZVelocity() - 0.1);
		}
		if (ball->getZVelocity() < 0.1 && ball->getZVelocity() > -0.1)
		{
			ball->updateZVelocity(0.0f);
		}
	}
};