#include <cgg/cgg.h>
#include "gl/Primitives.h"
#pragma once
#include "CollisionEntity.h"

/// /class Ball
/// Defines a sphere with physics

class Ball : public CollisionEntity
{
public:
	Ball(cgg::Mat43);
	Ball(cgg::Mat43, cgg::Vec3, float);
	void render(gl::Primitives*);
protected:
	float radius;
};