#include "Entity.h"

Entity::Entity(cgg::Mat43 transform) : transform(transform)
{

}
Entity::Entity(cgg::Mat43 transform, cgg::Vec3 colour) : transform(transform), colour(colour)
{

}

//Quick Hack for Demo Purposes
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
	cgg::Vec3 pos;
	pos.x = transform.w.x;
	pos.y = transform.w.y;
	pos.z = transform.w.z;
	return pos;
}
