#include "Entity.h"

Entity::Entity(cgg::Mat43 transform) : transform(transform)
{
	
}
Entity::Entity(cgg::Mat43 transform, cgg::Vec3 colour) : transform(transform), colour(colour)
{
	
}

Entity::Entity(cgg::Vec3 pos)
{
	transform = cgg::Mat43::kIdentity;
	updatePosition(pos);
}
Entity::Entity(cgg::Vec3 pos, cgg::Vec3 colour) : colour(colour)
{
	transform = cgg::Mat43::kIdentity;
	updatePosition(pos);
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

void Entity::updateColour(cgg::Vec3 changeTo)
{
	colour = changeTo;
}