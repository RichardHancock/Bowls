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

protected:
	
	/// cgg::Mat43 containing a transform (position,rotation,scale).
	cgg::Mat43 transform;

	/// cgg::Vec3 containing a colour's r,g,b values.
	cgg::Vec3 colour;

};

#endif