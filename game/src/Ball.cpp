#include "Ball.h"

Ball::Ball(cgg::Mat43 transform) : CollisionEntity(transform)
{
	radius = 1;
	colour.x = 1;
	colour.y = 1;
	colour.z = 0;
}

Ball::Ball(cgg::Mat43 transform, cgg::Vec3 colour, float radius) 
	: CollisionEntity(transform, colour), radius(radius)
{

}

void Ball::render(gl::Primitives* primitiveList)
{
	primitiveList->sphere(transform, colour, radius);
}

float Ball::getRadius()
{
	return radius;
}