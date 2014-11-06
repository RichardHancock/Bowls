#include <cgg/cgg.h>
#include "gl/Primitives.h"

#pragma once

class Entity
{
public:
	Entity(cgg::Mat43);
	Entity(cgg::Mat43, cgg::Vec3);
	virtual void render(gl::Primitives*) = 0;
protected:
	cgg::Mat43 transform;
	cgg::Vec3 colour;
};