#include "CollisionEntity.h"

CollisionEntity::CollisionEntity(cgg::Mat43 transform)
	: Entity(transform)
{
	velocities = { 0.0f, 0.0f, 0.0f };
}

CollisionEntity::CollisionEntity(cgg::Mat43 transform, cgg::Vec3 colour)
	: Entity(transform, colour)
{
	velocities = { 0.0f, 0.0f, 0.0f };
}

//Quick Hack for Demo Purposes
void CollisionEntity::changePosition(cgg::Vec3 adjustBy)
{
	transform.w.x += adjustBy.x;
	transform.w.y += adjustBy.y;
	transform.w.z += adjustBy.z;
}

cgg::Vec3 CollisionEntity::getPosition()
{
	cgg::Vec3 pos = { transform.w.x, transform.w.y, transform.w.z };
	return pos;
}

//velocities
void CollisionEntity::updateXVelocity(float x)
{
	velocities.x = x;
}

void CollisionEntity::updateYVelocity(float y)
{
	velocities.y = y;
}

void CollisionEntity::updateZVelocity(float z)
{
	velocities.z = z;
}

float CollisionEntity::getXVelocity()
{
	return velocities.x;
}

float CollisionEntity::getYVelocity()
{
	return velocities.y;
}

float CollisionEntity::getZVelocity()
{
	return velocities.z;
}