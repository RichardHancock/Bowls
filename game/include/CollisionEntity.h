#pragma once
#ifndef COLLISION_ENTITY_H
#define COLLISION_ENTITY_H

#include <cgg/cgg.h>
#include "Entity.h"

/**
@brief Class that represents an Entity with Collision

Class that contains an Entity with Collision, no class above this
should have physics code.

@todo Make this a proper pure virtual class (if possible)
*/
class CollisionEntity : public Entity
{
public:

	/**
	PURE VIRTUAL Creates an Entity with Collision

	PURE VIRTUAL Don't create this object

	@param cgg::Mat43 - The transformation matrix
	*/
	CollisionEntity(cgg::Mat43);

	/**
	PURE VIRTUAL Creates an Entity with Collision

	PURE VIRTUAL Don't create this object

	@param cgg::Mat43 - The transformation matrix
	@param cgg::Vec3 - Contains the colour r,g,b values
	*/
	CollisionEntity(cgg::Mat43, cgg::Vec3);

	/**
	PURE VIRTUAL Creates an Entity with Collision

	PURE VIRTUAL Don't create this object

	@param cgg::Vec3 - Position
	*/
	CollisionEntity(cgg::Vec3);

	/**
	PURE VIRTUAL Creates an Entity with Collision

	PURE VIRTUAL Don't create this object

	@param cgg::Vec3 - Position
	@param cgg::Vec3 - Contains the colour r,g,b values
	*/
	CollisionEntity(cgg::Vec3, cgg::Vec3);

	/**
	Change the velocity to the inputed value

	The inputed floats are the new values of the x velocity

	@param float x velocity.
	*/
	void updateXVelocity(float);

	/**
	Change the velocity to the inputed value

	The inputed floats are the new values of the y velocity

	@param float y velocity.
	*/
	void updateYVelocity(float);

	/**
	Change the velocity to the inputed value

	The inputed floats are the new values of the z velocity

	@param float z velocity.
	*/
	void updateZVelocity(float);

	/**
	Gets the velocity

	Gets the x velocity

	@returns float x velocity.
	*/
	float getXVelocity();

	/**
	Gets the velocity

	Gets the y velocity

	@returns float y velocity.
	*/
	float getYVelocity();

	/**
	Gets the velocity

	Gets the z velocity

	@returns float z velocity.
	*/
	float getZVelocity();

private:
	/// cgg::Vec3 containing the velocities of the Entity.
	cgg::Vec3 velocities;
};

#endif