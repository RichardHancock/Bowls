#include "Box.h"

Box::Box(cgg::Mat43 transform, bool inAxis)
	:CollisionEntity(transform)
{
	colour.x = 1;
	colour.y = 0;
	colour.z = 1;
	xAxis = inAxis;
}

Box::Box(cgg::Mat43 transform, cgg::Vec3 colour, cgg::Vec3 dimensions, bool inAxis)
	: CollisionEntity(transform, colour), dimensions(dimensions)
{
	xAxis = inAxis;
}

Box::Box(cgg::Mat43 transform, cgg::Vec3 colour, float w, float h, float d, bool inAxis)
	: CollisionEntity(transform, colour), dimensions(cgg::Vec3(w,h,d))
{
	xAxis = inAxis;
}

Box::Box(cgg::Vec3 pos)
	: CollisionEntity(pos)
{
	colour.x = 1;
	colour.y = 0;
	colour.z = 1;
}

Box::Box(cgg::Vec3 pos, cgg::Vec3 colour, cgg::Vec3 dimensions)
	: CollisionEntity(pos, colour), dimensions(dimensions)
{

}

Box::Box(cgg::Vec3 pos, cgg::Vec3 colour, float w, float h, float d)
	: CollisionEntity(pos, colour), dimensions(cgg::Vec3(w, h, d))
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

bool Box::xAxisCheck()
{
	return xAxis;
}