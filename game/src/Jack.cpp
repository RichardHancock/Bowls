#include "Jack.h"

Jack::Jack(cgg::Mat43 transform) : Ball(transform)
{
	radius = 0.5;
	colour.x = 1;
	colour.y = 1;
	colour.z = 0;
	maxThrowStrength = 2.5;
}

Jack::Jack(cgg::Mat43 transform, cgg::Vec3 colour, float radius)
	:Ball(transform, colour, radius)
{
	maxThrowStrength = 2.5;
}

Jack::Jack(cgg::Vec3 pos) 
	: Ball(pos)
{
	radius = 0.5;
	colour.x = 1;
	colour.y = 1;
	colour.z = 0;
	maxThrowStrength = 2.5;
}

Jack::Jack(cgg::Vec3 pos, cgg::Vec3 colour, float radius)
	: Ball(pos, colour, radius)
{
	maxThrowStrength = 2.5;
}

void Jack::render(gl::Primitives* primitiveList)
{
	primitiveList->sphere(transform, colour, radius);
}