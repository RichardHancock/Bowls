#include <cgg/cgg.h>
#pragma once
#include "Entity.h"

class CollisionEntity : public Entity
{
public:
	CollisionEntity(cgg::Mat43);
	CollisionEntity(cgg::Mat43, cgg::Vec3);
private:

};