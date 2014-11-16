#pragma once
#ifndef BOX_H
#define BOX_H

#include <cgg/cgg.h>
#include "CollisionEntity.h"

/**
@brief Class that represents a Box

Class that contains a Box and its interactions with the game.
*/
class Box : public CollisionEntity
{
public:

	/**
	BROKEN - Create a Box object

	BROKEN - Creates a Box object using the passed in transform.

	@todo Find a way to copy the transform into the dimension Vec3 otherwise this crashes.
	@param cgg::Mat43 The transformation matrix
	*/
	Box(cgg::Mat43);

	/**
	Create a Box object

	Creates a Box object using the passed in transform, colour and dimensions.

	@param cgg::Mat43 - The transformation matrix
	@param cgg::Vec3 - Contains the colour r,g,b values
	@param cgg::Vec3 - Dimensions contains the width, height and depth (In that order)
	*/
	Box(cgg::Mat43, cgg::Vec3, cgg::Vec3);

	/**
	Create a Box object

	Creates a Box object using the passed in transform, colour and individual dimensions.

	@param cgg::Mat43 - The transformation matrix
	@param cgg::Vec3 - Contains the colour r,g,b values
	@param float - Width of the Box
	@param float - Height of the Box
	@param float - Depth of the Box
	*/
	Box(cgg::Mat43, cgg::Vec3, float, float, float);

	/**
	Renders a box

	Creates a box using the objects internal values and passes it into primitiveList.

	@param gl::Primitives* - Pointer to a list of primitives
	*/
	void render(gl::Primitives*);

protected:
	
	/// Contains the Width, Height and Depth of the Box
	cgg::Vec3 dimensions;
};

#endif