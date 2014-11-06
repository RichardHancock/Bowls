#include "Ball.h"

Ball::Ball(cgg::Mat43 transform) : CollisionEntity(transform)
{
	
}

Ball::Ball(cgg::Mat43 transform, cgg::Vec3 colour, float radius) 
	: CollisionEntity(transform, colour), radius(radius)
{

}

/// /brief Renders the Ball object
///
/// Uses the passed in g_prims to save a sphere object with the 
/// internal transform, colour and radius.
void Ball::render(gl::Primitives* g_prims)
{
	g_prims->sphere(transform, colour, radius);
}