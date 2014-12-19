#pragma once

#include <math.h>
#include "cgg/cgg.h"
#include "Bowl.h"
#include "Box.h"

/**
@brief Physics functions

A Namespace that contains functions for the physics in the game.
Code by Jamie Slowgrove
*/
namespace Physics
{
	/**
	Checks if 2 balls collide

	Takes in two balls and a delta time and uses this to detect if the balls collide

	@param Ball* one of the balls to be used in the collision
	@param Ball* one of the balls to be used in the collision
	@param float the current delta time
	@return bool if the collision occurs
	*/
	bool collisionCheck(Ball* ball1, Ball* ball2, float dt)
	{
		// gets the combination of the 2 balls radius
		float radSum = ball1->getRadius() + ball2->getRadius();
		// stores the vec3's positions of the balls as local variables
		cgg::Vec3 ball1Pos = ball1->getPosition();
		cgg::Vec3 ball2Pos = ball2->getPosition();
		// work out the difference in the coordinates of the 2 spheres for the next update
		float deltaX = (ball1Pos.x + (ball1->getXVelocity() * dt)) - (ball2Pos.x + (ball2->getXVelocity() * dt));
		float deltaY = (ball1Pos.y + (ball1->getYVelocity() * dt)) - (ball2Pos.y + (ball2->getYVelocity() * dt));
		float deltaZ = (ball1Pos.z + (ball1->getZVelocity() * dt)) - (ball2Pos.z + (ball2->getZVelocity() * dt));
		// sqrt (dX^2 + dY^2 + dZ^2)
		float distance = sqrtf((deltaX * deltaX) + (deltaY * deltaY) + (deltaZ * deltaZ));
		//if the distance between the two balls is less than the sum of the radius's then there will be a collison
		if (distance < radSum)
		{
			return true;
		}
		return false;
	}

	/**
	Checks if a ball collides with a wall

	Takes in a ball, a wall and a delta time and uses this to detect if the two collide

	@param Ball* the ball to be used in the collision
	@param Box* the wall to be used in the collision
	@param float the current delta time
	@return bool if the collision occurs
	*/
	bool collisionCheck(Ball* ball, Box* wall, float dt)
	{		
		//local variables contain the values to be used with the calculation
		float rad = ball->getRadius();
		//value is halved to make it work with the coordinates
		float width = (wall->getWidth()*0.5);
		float depth = (wall->getDepth()*0.5);
		cgg::Vec3 ballPos = ball->getPosition();
		//the position of the ball next frame
		ballPos.x += (ball->getXVelocity() * dt);
		ballPos.z += (ball->getZVelocity() * dt);
		cgg::Vec3 wallPos = wall->getPosition();
		//check if the position of the ball is inside the wall
		if (ballPos.x + rad < wallPos.x + width && ballPos.x + rad > wallPos.x - width
			&& ballPos.z + rad < wallPos.z + depth && ballPos.z + rad > wallPos.z - depth
			|| ballPos.x - rad < wallPos.x + width && ballPos.x - rad > wallPos.x - width
			&& ballPos.z - rad < wallPos.z + depth && ballPos.z - rad > wallPos.z - depth)
		{
			return true;
		}
		return false;
	}

	/**
	Works out the new velocities after the balls collide

	Takes in two balls works out the new velocities of the balls

	@param Ball* one of the balls to be updated
	@param Ball* one of the balls to be updated
	*/
	void newCollisionVelocities(Ball* ball1, Ball* ball2)
	{
		//if a velocity is 0 then update the velocity early so that it wont cause an error
		if (ball1->getXVelocity() == 0)
		{
			//set the ball with 0 velocity to the second velocity 
			ball1->updateXVelocity((ball2->getXVelocity()));
			//decrease the velocity of the ball to appear as if some was lost on impact
			ball2->updateXVelocity((ball2->getXVelocity()*0.75f));
		}
		//if a velocity is 0 then update the velocity early so that it wont cause an error
		if (ball1->getZVelocity() == 0)
		{
			//set the ball with 0 velocity to the second velocity 
			ball1->updateZVelocity((ball2->getZVelocity()));
			//decrease the velocity of the ball to appear as if some was lost on impact
			ball2->updateZVelocity((ball2->getZVelocity()*0.75f));
		}
		//if a velocity is 0 then update the velocity early so that it wont cause an error
		if (ball2->getXVelocity() == 0)
		{
			//set the ball with 0 velocity to the second velocity 
			ball2->updateXVelocity((ball1->getXVelocity()));
			//decrease the velocity of the ball to appear as if some was lost on impact
			ball1->updateXVelocity((ball1->getXVelocity()*0.75f));
		}
		//if a velocity is 0 then update the velocity early so that it wont cause an error
		if (ball2->getZVelocity() == 0)
		{
			//set the ball with 0 velocity to the second velocity 
			ball2->updateZVelocity((ball1->getZVelocity()));
			//decrease the velocity of the ball to appear as if some was lost on impact
			ball1->updateZVelocity((ball1->getZVelocity()*0.75f));
		}

		//create local variables of the 1st balls velocities so that the second ball can use them
		float tmpBall1X = ball1->getXVelocity();
		float tmpBall1Z = ball1->getZVelocity();

		//sets the 1st ball velocities to the value of 2nd ball velocities with a decrease velocity from impact
		ball1->updateXVelocity((ball2->getXVelocity()*0.75f));
		ball1->updateZVelocity((ball2->getZVelocity()*0.75f));
		//sets the 2nd ball velocities to the value of 1st ball velocities with a decrease velocity from impact
		ball2->updateXVelocity((tmpBall1X*0.75f));
		ball2->updateZVelocity((tmpBall1Z*0.75f));
	}

	/**
	Works out the new velocities after the ball collides with a wall

	Takes in the ball and wall and works out the new velocities of the ball

	@param Ball* the ball to be updated
	@param Box* the wall it has collided with
	*/
	void newCollisionVelocities(Ball* ball, Box* wall)
	{
		//check if the wall is running along the z axis
		if (!wall->xAxisCheck())
		{
			//invert the z velocity
			ball->updateZVelocity(-(ball->getZVelocity()));
		}
		else
		{
			//invert the x velocity
			ball->updateXVelocity(-(ball->getXVelocity()));
		}
	}

	/**
	Applies Friction to the ball

	Takes in the ball and adds friction to its velocities

	@param Ball* the ball to be updated
	*/
	void applyFriction(Ball* ball)
	{
		// if the x velocity is less than 0
		if (ball->getXVelocity() < 0)
		{
			//add 0.1 to decrease the velocity
			ball->updateXVelocity(ball->getXVelocity() + 0.1);
		}
		// if the x velocity is greater than 0
		if (ball->getXVelocity() > 0)
		{
			//subtract 0.1 to decrease the velocity
			ball->updateXVelocity(ball->getXVelocity() - 0.1);
		}
		//if the x velocity is between 0.1 and - 0.1
		if (ball->getXVelocity() < 0.1 && ball->getXVelocity() > -0.1)
		{
			//set the x velocity to 0
			ball->updateXVelocity(0.0f);
		}

		// if the z velocity is less than 0
		if (ball->getZVelocity() < 0)
		{
			//add 0.1 to decrease the velocity
			ball->updateZVelocity(ball->getZVelocity() + 0.1);
		}
		// if the z velocity is greater than 0
		if (ball->getZVelocity() > 0)
		{
			//subtract 0.1 to decrease the velocity
			ball->updateZVelocity(ball->getZVelocity() - 0.1);
		}
		//if the z velocity is between 0.1 and - 0.1
		if (ball->getZVelocity() < 0.1 && ball->getZVelocity() > -0.1)
		{
			//set the z velocity to 0
			ball->updateZVelocity(0.0f);
		}
	}

	/**
	Works out the velocity from the kinect input

	Takes in the change in distance and change in time from the kinect output and converts it to a velocity

	@param float the change in distance
	@param float the change in time
	@returns float the calculated velocity
	*/
	float kinectInputVelocity(float distance, float time)
	{
		//velocity = change in distance/ change in time
		float velocity = distance / time;
		return velocity;
	}
};