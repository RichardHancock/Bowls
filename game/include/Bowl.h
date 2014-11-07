#include <cgg/cgg.h>
#pragma once
#include "Ball.h"

/** 
@brief Class that represents a bowl

Class that contains a bowl and its interactions with the game.
*/
class Bowl : public Ball
{
public:

	/**
	Create a Bowl object

	Creates a Bowl object using the passed in transform. Colour and radius are set to internal defaults.

	@param transform The transformation matrix
	*/
	Bowl(cgg::Mat43);

	/** 
	Create a Bowl object

	Creates a Bowl object using the passed in transform, colour and radius.

	@param transform - The transformation matrix
	@param colour - Vec3 containing the r,g,b values
	@param radius - Float value containing the radius of the sphere
	*/
	Bowl(cgg::Mat43, cgg::Vec3, float);

	/** 
	Renders a sphere
	
	Creates a sphere using the objects internal values and passes it into primitiveList.

	@param primitiveList* - Pointer to a list of primitives
	*/
	void render(gl::Primitives*);
};