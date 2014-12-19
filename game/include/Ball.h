#pragma once
#ifndef BALL_H
#define BALL_H

#include <cgg/cgg.h>
#include "gl/Primitives.h"
#include "CollisionEntity.h"

/**
@brief Class that represents a Ball

Class that contains a ball and its interactions with the game.
*/
class Ball : public CollisionEntity
{
public:

	/**
	Create a Ball object

	Creates a Ball object using the passed in transform. Colour and radius are set to internal defaults.

	@param cgg::Mat43 The transformation matrix
	*/
	Ball(cgg::Mat43);

	/**
	Create a Ball object

	Creates a Ball object using the passed in transform, colour and radius.

	@param cgg::Mat43 - The transformation matrix
	@param cgg::Vec3 - Contains the colour r,g,b values
	@param float - Float value containing the radius of the sphere
	*/
	Ball(cgg::Mat43, cgg::Vec3, float);

	/**
	Renders a sphere

	Creates a sphere using the objects internal values and passes it into primitiveList.

	@param gl::Primitives* - Pointer to a list of primitives
	*/
	void render(gl::Primitives*);

	float getRadius();

	float getThrow();

protected:

	/// Contains the radius of the sphere
	float radius;

	float maxThrowStrength;
};

#endif