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

	@param cgg::Mat43 The transformation matrix
	*/
	CollisionEntity(cgg::Mat43);

	/**
	PURE VIRTUAL Creates an Entity with Collision

	PURE VIRTUAL Don't create this object

	@param cgg::Mat43 - The transformation matrix
	@param cgg::Vec3 - Contains the colour r,g,b values
	*/
	CollisionEntity(cgg::Mat43, cgg::Vec3);
private:

};

#endif