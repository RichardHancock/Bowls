#include "CollisionEntity.h"

CollisionEntity::CollisionEntity(cgg::Mat43 transform)
	: Entity(transform)
{

}

CollisionEntity::CollisionEntity(cgg::Mat43 transform, cgg::Vec3 colour)
	: Entity(transform, colour)
{

}

CollisionEntity::CollisionEntity(cgg::Vec3 pos)
	: Entity(pos)
{

}
CollisionEntity::CollisionEntity(cgg::Vec3 pos, cgg::Vec3 colour)
	: Entity(pos, colour)
{
	
}