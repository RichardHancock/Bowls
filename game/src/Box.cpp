#include "Box.h"

Box::Box(cgg::Mat43 transform)
	:CollisionEntity(transform)
{
	colour.x = 1;
	colour.y = 0;
	colour.z = 1;
}

Box::Box(cgg::Mat43 transform, cgg::Vec3 colour, cgg::Vec3 dimensions)
	: CollisionEntity(transform, colour), dimensions(dimensions)
{

}

Box::Box(cgg::Mat43 transform, cgg::Vec3 colour, float w, float h, float d)
	: CollisionEntity(transform, colour), dimensions(cgg::Vec3(w,h,d))
{

}

void Box::render(gl::Primitives* primitiveList)
{
	primitiveList->box(transform,colour,dimensions.x,dimensions.y,dimensions.z);
}

float Box::getWidth()
{
	return dimensions.getX();
}

float Box::getDepth()
{
	return dimensions.getZ();
}

float Box::getHeight()
{
	return dimensions.getY();
}