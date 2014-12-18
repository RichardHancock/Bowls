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