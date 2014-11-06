#include "Jack.h"

Jack::Jack(cgg::Mat43 transform) : Ball(transform)
{
	radius = 0.5;
	colour.x = 1;
	colour.y = 1;
	colour.z = 0;
}

Jack::Jack(cgg::Mat43 transform, cgg::Vec3 colour, float radius)
	:Ball(transform, colour, radius)
{
	
}

void Jack::render(gl::Primitives* g_prims)
{
	g_prims->sphere(transform, colour, radius);
}