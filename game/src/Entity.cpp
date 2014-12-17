#include "Entity.h"

Entity::Entity(cgg::Mat43 transform) : transform(transform)
{
	velocities = { 0.0f, 0.0f, 0.0f };
}
Entity::Entity(cgg::Mat43 transform, cgg::Vec3 colour) : transform(transform), colour(colour)
{
	velocities = { 0.0f, 0.0f, 0.0f };
}

//Quick Hack for Demo Purposes
void Entity::changePosition(cgg::Vec3 adjustBy)
{
	transform.w.x += adjustBy.x;
	transform.w.y += adjustBy.y;
	transform.w.z += adjustBy.z;
}

cgg::Vec3 Entity::getPosition()
{
	cgg::Vec3 pos = { transform.w.x, transform.w.y, transform.w.z};
	return pos;
}

//velocities
void Entity::updateXVelocity(float x)
{
	velocities.x = x;
}

void Entity::updateYVelocity(float y)
{
	velocities.y = y;
}

void Entity::updateZVelocity(float z)
{
	velocities.z = z;
}

float Entity::getXVelocity()
{
	return velocities.x;
}

float Entity::getYVelocity()
{
	return velocities.y;
}

float Entity::getZVelocity()
{
	return velocities.z;
}

