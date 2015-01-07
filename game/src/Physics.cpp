#include "Physics.h"

/*
Works out the distance between two spheres
*/
float Physics::distanceBetweenTwoSpheres(cgg::Vec3 sphere1Pos, cgg::Vec3 sphere2Pos)
{
	// work out the difference in the coordinates of the 2 spheres
	float deltaX = sphere1Pos.x - sphere2Pos.x;
	float deltaY = sphere1Pos.y - sphere2Pos.y;
	float deltaZ = sphere1Pos.z - sphere2Pos.z;
	// sqrt (dX^2 + dY^2 + dZ^2)
	float distance = sqrtf((deltaX * deltaX) + (deltaY * deltaY) + (deltaZ * deltaZ));
	return distance;
}

/*
Checks if 2 spheres collide
*/
bool Physics::collisionCheck(Ball* sphere1, Ball* sphere2, float dt)
{
	// gets the combination of the 2 spheres radius
	float radSum = sphere1->getRadius() + sphere2->getRadius();
	// stores the vec3's positions of the spheres as local variables and sets them for what they will be next frame
	cgg::Vec3 sphere1Pos = sphere1->getPosition();
	sphere1Pos.x += (sphere1->getXVelocity() * dt);
	sphere1Pos.y += (sphere1->getYVelocity() * dt);
	sphere1Pos.z += (sphere1->getZVelocity() * dt);
	cgg::Vec3 sphere2Pos = sphere2->getPosition();
	sphere2Pos.x += (sphere2->getXVelocity() * dt);
	sphere2Pos.y += (sphere2->getYVelocity() * dt);
	sphere2Pos.z += (sphere2->getZVelocity() * dt);
	//work out the distance between the spheres
	float distance = distanceBetweenTwoSpheres(sphere1Pos, sphere2Pos);
	//if the distance between the two spheres is less than the sum of the radius's then there will be a collision
	if (distance < radSum)
	{
		return true;
	}
	return false;
}

/*
Checks if a sphere collides with a wall
*/
bool Physics::collisionCheck(Ball* sphere, Box* wall, float dt)
{
	//local variables contain the values to be used with the calculation
	float rad = sphere->getRadius();
	//value is halved to make it work with the coordinates
	float width = (wall->getWidth()*0.5);
	float depth = (wall->getDepth()*0.5);
	cgg::Vec3 spherePos = sphere->getPosition();
	//the position of the sphere next frame
	spherePos.x += (sphere->getXVelocity() * dt);
	spherePos.z += (sphere->getZVelocity() * dt);
	cgg::Vec3 wallPos = wall->getPosition();
	//check if the position of the sphere is inside the wall
	if (spherePos.x + rad < wallPos.x + width && spherePos.x + rad > wallPos.x - width
		&& spherePos.z + rad < wallPos.z + depth && spherePos.z + rad > wallPos.z - depth
		|| spherePos.x - rad < wallPos.x + width && spherePos.x - rad > wallPos.x - width
		&& spherePos.z - rad < wallPos.z + depth && spherePos.z - rad > wallPos.z - depth)
	{
		return true;
	}
	return false;
}

/*
Works out the new velocities after the spheres collide
*/
void Physics::newCollisionVelocities(Ball* sphere1, Ball* sphere2)
{
	//if a velocity is 0 then update the velocity early so that it wont cause an error
	if (sphere1->getXVelocity() == 0)
	{
		//set the sphere with 0 velocity to the second velocity 
		sphere1->updateXVelocity((sphere2->getXVelocity()));
		//decrease the velocity of the sphere to appear as if some was lost on impact
		sphere2->updateXVelocity((sphere2->getXVelocity()*0.75f));
	}
	//if a velocity is 0 then update the velocity early so that it wont cause an error
	if (sphere1->getZVelocity() == 0)
	{
		//set the sphere with 0 velocity to the second velocity 
		sphere1->updateZVelocity((sphere2->getZVelocity()));
		//decrease the velocity of the sphere to appear as if some was lost on impact
		sphere2->updateZVelocity((sphere2->getZVelocity()*0.75f));
	}
	//if a velocity is 0 then update the velocity early so that it wont cause an error
	if (sphere2->getXVelocity() == 0)
	{
		//set the sphere with 0 velocity to the second velocity 
		sphere2->updateXVelocity((sphere1->getXVelocity()));
		//decrease the velocity of the sphere to appear as if some was lost on impact
		sphere1->updateXVelocity((sphere1->getXVelocity()*0.75f));
	}
	//if a velocity is 0 then update the velocity early so that it wont cause an error
	if (sphere2->getZVelocity() == 0)
	{
		//set the sphere with 0 velocity to the second velocity 
		sphere2->updateZVelocity((sphere1->getZVelocity()));
		//decrease the velocity of the sphere to appear as if some was lost on impact
		sphere1->updateZVelocity((sphere1->getZVelocity()*0.75f));
	}

	//create local variables of the 1st spheres velocities so that the second sphere can use them
	float tmpSphere1X = sphere1->getXVelocity();
	float tmpSphere1Z = sphere1->getZVelocity();

	//sets the 1st sphere velocities to the value of 2nd sphere velocities with a decrease velocity from impact
	sphere1->updateXVelocity((sphere2->getXVelocity()*0.75f));
	sphere1->updateZVelocity((sphere2->getZVelocity()*0.75f));
	//sets the 2nd sphere velocities to the value of 1st sphere velocities with a decrease velocity from impact
	sphere2->updateXVelocity((tmpSphere1X*0.75f));
	sphere2->updateZVelocity((tmpSphere1Z*0.75f));
}

/*
Works out the new velocities after the sphere collides with a wall
*/
void Physics::newCollisionVelocities(Ball* sphere, Box* wall)
{
	//check if the wall is running along the z axis
	if (!wall->xAxisCheck())
	{
		//invert the z velocity
		sphere->updateZVelocity(-(sphere->getZVelocity()));
	}
	else
	{
		//invert the x velocity
		sphere->updateXVelocity(-(sphere->getXVelocity()));
	}
}

/*
Applies Friction to the sphere
*/
void Physics::applyFriction(Ball* sphere)
{
	// if the x velocity is less than 0
	if (sphere->getXVelocity() < 0)
	{
		//add 0.1 to decrease the velocity
		sphere->updateXVelocity(sphere->getXVelocity() + 0.1);
	}
	// if the x velocity is greater than 0
	if (sphere->getXVelocity() > 0)
	{
		//subtract 0.1 to decrease the velocity
		sphere->updateXVelocity(sphere->getXVelocity() - 0.1);
	}
	//if the x velocity is between 0.1 and - 0.1
	if (sphere->getXVelocity() < 0.1 && sphere->getXVelocity() > -0.1)
	{
		//set the x velocity to 0
		sphere->updateXVelocity(0.0f);
	}

	// if the z velocity is less than 0
	if (sphere->getZVelocity() < 0)
	{
		//add 0.1 to decrease the velocity
		sphere->updateZVelocity(sphere->getZVelocity() + 0.1);
	}
	// if the z velocity is greater than 0
	if (sphere->getZVelocity() > 0)
	{
		//subtract 0.1 to decrease the velocity
		sphere->updateZVelocity(sphere->getZVelocity() - 0.1);
	}
	//if the z velocity is between 0.1 and - 0.1
	if (sphere->getZVelocity() < 0.1 && sphere->getZVelocity() > -0.1)
	{
		//set the z velocity to 0
		sphere->updateZVelocity(0.0f);
	}
}

/*
Works out the velocity from the kinect input
*/
float Physics::kinectInputVelocity(float distance, float time)
{
	//velocity = change in distance/ change in time
	float velocity = distance / time;
	return velocity;
}