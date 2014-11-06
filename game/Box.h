#include <cgg/cgg.h>

#include "CollisionEntity.h"

class Box : public CollisionEntity
{
public:
	Box(cgg::Mat43);
	Box(cgg::Mat43, cgg::Vec3, cgg::Vec3);
	Box(cgg::Mat43, cgg::Vec3, float, float, float);
	void render(gl::Primitives*);
protected:
	cgg::Vec3 dimensions;
};