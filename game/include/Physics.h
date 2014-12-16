#pragma once

/**
@brief Physics functions

A Namespace that contains functions for the physics in the game.
*/
namespace Physics
{
	bool quadraticFormula(float a, float b, float c, float& x0, float& x1)
	{
		float term = (b*b) - (4.0f*a*c);
		if (term >= 0)
		{
			term = sqrt(term);
			x0 = (-b - term) / (2.0f*a);
			x1 = (-b + term) / (2.0f*a);
			return true;
		}
		// no real solutions exist,
		// the square root is negative!
		return false;
	}

	bool ballHitBallCheck(cgg::Vec3 Pa, cgg::Vec3 Da, cgg::Vec3 Ra, cgg::Vec3 Pb, 
							cgg::Vec3 Db, cgg::Vec3 Rb, float& t0, float& t1)
	{
		cgg::Vec3 P = Pa - Pb;
		cgg::Vec3 D = Da - Db;
		cgg::Vec3 R = Ra + Rb;
		float a = cgg::dot(D, D);
		float b = 2 * cgg::dot(D, P);
		float c = cgg::dot(P, P) - cgg::dot(R, R); //...............Hopefully?
		return quadraticFormula(a, b, c, t0, t1);
	}
};