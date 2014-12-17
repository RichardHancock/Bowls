#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include <cgg/cgg.h>
#include "gl/Primitives.h"

/**
@brief Class that represents an Entity

Class that contains an Entity, which is any object that appears in the game world.
They have a position and colour.
*/
class Entity
{
public:

	/**
	PURE VIRTUAL Creates an Entity

	PURE VIRTUAL Don't create this object

	@param cgg::Mat43 The transformation matrix
	*/
	Entity(cgg::Mat43);

	/**
	PURE VIRTUAL Creates an Entity

	PURE VIRTUAL Don't create this object

	@param cgg::Mat43 - The transformation matrix
	@param cgg::Vec3 - Contains the colour r,g,b values
	*/
	Entity(cgg::Mat43, cgg::Vec3);

	/**
	Renders a primitive

	Creates a primitive and passes it into primitiveList.

	@param gl::Primitives* - Pointer to a list of primitives
	*/
	virtual void render(gl::Primitives*) = 0;

	/** 
	Change the position by this amount

	The inputed Vec3 is +='d to the original transform 

	@param cgg::Vec3 - x,y,z to adjust by.
	*/
	void changePosition(cgg::Vec3);

	/**
	Gets the current position

	Gets the current position in a cgg::Vec3

	@returns cgg::Vec3 - Current Position. 
	*/
	cgg::Vec3 getPosition();

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
protected:
	
	/// cgg::Mat43 containing a transform (position,rotation,scale).
	cgg::Mat43 transform;

	/// cgg::Vec3 containing a colour's r,g,b values.
	cgg::Vec3 colour;

	/// cgg::Vec3 containing the velocities of the Entity.
	cgg::Vec3 velocities;
};

#endif