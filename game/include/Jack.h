#pragma once
#ifndef JACK_H
#define JACK_H

#include <cgg/cgg.h>
#include "Ball.h"

/**
@brief Class that represents a Jack

Class that contains a Jack and its interactions with the game.
*/
class Jack : public Ball
{
public:
	/**
	Create a Jack object

	Creates a Jack object using the passed in transform. Colour and radius are set to internal defaults.

	@param cgg::Mat43 The transformation matrix
	*/
	Jack(cgg::Mat43);

	/**
	Create a Jack object

	Creates a Jack object using the passed in transform, colour and radius.

	@param cgg::Mat43 - The transformation matrix
	@param cgg::Vec3 - Contains the colour r,g,b values
	@param float - Float value containing the radius of the sphere
	*/
	Jack(cgg::Mat43, cgg::Vec3, float);

	/**
	Renders a sphere

	Creates a sphere using the objects internal values and passes it into primitiveList.

	@param gl::Primitives* - Pointer to a list of primitives
	*/
	void render(gl::Primitives*);
};

#endif