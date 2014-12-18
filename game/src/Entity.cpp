#include "Entity.h"

Entity::Entity(cgg::Mat43 transform) : transform(transform)
{
	
}
Entity::Entity(cgg::Mat43 transform, cgg::Vec3 colour) : transform(transform), colour(colour)
{
	
}

void Entity::changePosition(cgg::Vec3 adjustBy)
{
	transform.w.x += adjustBy.x;
	transform.w.y += adjustBy.y;
	transform.w.z += adjustBy.z;
}

void Entity::updatePosition(cgg::Vec3 changeTo)
{
	transform.w.x = changeTo.x;
	transform.w.y = changeTo.y;
	transform.w.z = changeTo.z;
}

cgg::Vec3 Entity::getPosition()
{
	cgg::Vec3 pos = { transform.w.x, transform.w.y, transform.w.z };
	return pos;
}

