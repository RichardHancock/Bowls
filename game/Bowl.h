#include <cgg/cgg.h>
#pragma once
#include "Ball.h"

class Bowl : public Ball
{
public:
	Bowl(cgg::Mat43);
	Bowl(cgg::Mat43, cgg::Vec3, float);
	void render(gl::Primitives*);
};