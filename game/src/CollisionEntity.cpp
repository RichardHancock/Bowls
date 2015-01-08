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

CollisionEntity::CollisionEntity(cgg::Vec3 pos)
	: Entity(pos)
{
	velocities = { 0.0f, 0.0f, 0.0f };
}
CollisionEntity::CollisionEntity(cgg::Vec3 pos, cgg::Vec3 colour)
	: Entity(pos, colour)
{
	velocities = { 0.0f, 0.0f, 0.0f };
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