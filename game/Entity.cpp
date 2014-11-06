#include "Entity.h"

Entity::Entity(cgg::Mat43 transform) : transform(transform)
{

}
Entity::Entity(cgg::Mat43 transform, cgg::Vec3 colour) : transform(transform), colour(colour)
{

}


