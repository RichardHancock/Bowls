#include <cgg/cgg.h>
#pragma once
#include "Ball.h"

class Jack : public Ball
{
public:
	Jack(cgg::Mat43);
	Jack(cgg::Mat43, cgg::Vec3, float);
	void render(gl::Primitives*);
};