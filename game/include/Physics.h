#pragma once

#include <math.h>
#include "cgg/cgg.h"
#include "Bowl.h"
#include "Box.h"

/**
@brief Physics functions

A Namespace that contains functions for the physics in the game.
*/
namespace Physics
{
	bool collisionCheck(Ball* ball1, Ball* ball2, float dt)
	{
		float radSum = ball1->getRadius() + ball2->getRadius();
		cgg::Vec3 ball1Pos = ball1->getPosition();
		cgg::Vec3 ball2Pos = ball2->getPosition();
		float deltaX = (ball1Pos.x + (ball1->getXVelocity() * dt)) - (ball2Pos.x + (ball2->getXVelocity() * dt));
		float deltaY = (ball1Pos.y + (ball1->getYVelocity() * dt)) - (ball2Pos.y + (ball2->getYVelocity() * dt));
		float deltaZ = (ball1Pos.z + (ball1->getZVelocity() * dt)) - (ball2Pos.z + (ball2->getZVelocity() * dt));
		// sqrt (dX^2 + dY^2 + dZ^2)
		float distance = sqrtf((deltaX * deltaX) + (deltaY * deltaY) + (deltaZ * deltaZ));
		if (distance < radSum)
		{
			return true;
		}
		return false;
	}

	bool collisionCheck(Ball* ball, Box* wall, float dt)
	{		
		float rad = ball->getRadius();
		float width = (wall->getWidth()*0.5);
		float depth = (wall->getDepth()*0.5);
		cgg::Vec3 ballPos = ball->getPosition();
		ballPos.x += (ball->getXVelocity() * dt);
		ballPos.z += (ball->getZVelocity() * dt);
		cgg::Vec3 wallPos = wall->getPosition();
		if (ballPos.x + rad < wallPos.x + width && ballPos.x + rad > wallPos.x - width
			&& ballPos.z + rad < wallPos.z + depth && ballPos.z + rad > wallPos.z - depth
			|| ballPos.x - rad < wallPos.x + width && ballPos.x - rad > wallPos.x - width
			&& ballPos.z - rad < wallPos.z + depth && ballPos.z - rad > wallPos.z - depth)
		{
			return true;
		}
		return false;
	}

	void newCollisionVelocities(Ball* ball1, Ball* ball2)
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

		float tmpBall1X = ball1->getXVelocity();
		float tmpBall1Z = ball1->getZVelocity();

		ball1->updateXVelocity((ball2->getXVelocity()*0.5f));
		ball1->updateZVelocity((ball2->getZVelocity()*0.5f));
		ball2->updateXVelocity((tmpBall1X*0.5f));
		ball2->updateZVelocity((tmpBall1Z*0.5f));
	}

	void newCollisionVelocities(Ball* ball)
	{
		ball->updateXVelocity(-(ball->getXVelocity()));
		ball->updateZVelocity(-(ball->getZVelocity()));
	}

	void applyFriction(Ball* ball)
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

	float kinectInputVelocity(float distance, float time)
	{
		float velocity = distance / time;
		return velocity;
	}
};