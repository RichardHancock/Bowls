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
	Works out the distance between two spheres

	Takes in two sphere positions and works out the distance between them

	@param cgg::Vec3 one of the position of the spheres to be used in the collision
	@param cgg::Vec3 one of the position of the spheres to be used in the collision
	@param float the current delta time
	@return bool if the collision occurs
	*/
	float distanceBetweenTwoSpheres(cgg::Vec3 sphere1Pos, cgg::Vec3 sphere2Pos);

	/**
	Checks if 2 spheres collide

	Takes in two spheres and a delta time and uses this to detect if the spheres collide

	@param Ball* one of the spheres to be used in the collision
	@param Ball* one of the spheres to be used in the collision
	@param float the current delta time
	@return bool if the collision occurs
	*/
	bool collisionCheck(Ball* sphere1, Ball* sphere2, float dt);

	/**
	Checks if a sphere collides with a wall

	Takes in a sphere, a wall and a delta time and uses this to detect if the two collide

	@param Ball* the sphere to be used in the collision
	@param Box* the wall to be used in the collision
	@param float the current delta time
	@return bool if the collision occurs
	*/
	bool collisionCheck(Ball* sphere, Box* wall, float dt);

	/**
	Works out the new velocities after the spheres collide

	Takes in two spheres works out the new velocities of the spheres

	@param Ball* one of the spheres to be updated
	@param Ball* one of the spheres to be updated
	*/
	void newCollisionVelocities(Ball* sphere1, Ball* sphere2);

	/**
	Works out the new velocities after the sphere collides with a wall

	Takes in the sphere and wall and works out the new velocities of the sphere

	@param Ball* the sphere to be updated
	@param Box* the wall it has collided with
	*/
	void newCollisionVelocities(Ball* sphere, Box* wall);

	/**
	Applies Friction to the sphere

	Takes in the sphere and adds friction to its velocities

	@param Ball* the sphere to be updated
	*/
	void applyFriction(Ball* sphere);

	/**
	Works out the velocity from the kinect input

	Takes in the change in distance and change in time from the kinect output and converts it to a velocity

	@param float the change in distance
	@param float the change in time
	@returns float the calculated velocity
	*/
	float kinectInputVelocity(float distance, float time);
};