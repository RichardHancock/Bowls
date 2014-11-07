#include <cgg/cgg.h>
#pragma once
#include "Ball.h"

/// /class Bowl
/// Defines a Bowl

class Bowl : public Ball
{
public:

	/// /param transform The transformation matrix
	Bowl(cgg::Mat43);

	/// /param transform The transformation matrix
	/// /param colour    Vec3 containing the r,g,b values
	/// /param radius    Float value containing the radius of the sphere
	Bowl(cgg::Mat43, cgg::Vec3, float);

	/// /brief Renders a sphere
	/// /param primitiveList*  Pointer to a list of primitives
	/// 
	/// Creates a sphere using the objects internal values and passes it into primitiveList.
	void render(gl::Primitives*);
};