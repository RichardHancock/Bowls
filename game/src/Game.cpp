#include "Game.h"

Game::Game(bool sitMode, TrackingPoint hand)
{
	srand((unsigned int)time(NULL));
	kinectSensor = new KinectInput(sitMode, hand);
	stage = 0; //the current cue stage
	lockedZ = 0.0f; // the z position to be used with input maths
	lockedX = 0.0f;
}

Game::~Game()
{
	for (int i = 0; i < redBowls.size(); i++)
	{
		delete redBowls[i];
	}
	
	for (int i = 0; i < blueBowls.size(); i++)
	{
		delete blueBowls[i];
	}

	delete jack;
}

void Game::update(float dt)
{
	kinectSensor->update();
	cgg::Vec3 handPos = kinectSensor->getHandPos();

	switch (stage)//moves the kinect on screen differently depending on the stage
	{
	case 0:
		// Kinect Input - moves in z axis
		jack->updatePosition(cgg::Vec3(-17, -1, 0) + cgg::Vec3(-5, -1.5f, handPos.z));
		break;
	case 1:
	case 2:
		// Kinect Input - moves in x axis
		jack->updatePosition(cgg::Vec3(-17, -1, 0) + cgg::Vec3(handPos.x, -1.5, lockedZ));
		break;
	}

	//jack->updatePosition(cgg::Vec3(-17, -1, 0) + handPos);

	//g_camera.track(jack->getPosition().x, jack->getPosition().y);

	if (cgg::isKeyPressed(cgg::kKeyEscape))
	{
		exit(0);
	}
	if (cgg::isKeyPressed(cgg::kKeyEnter))
	{
		//reset the game
		stage = 0;
		resetPositions();
		Timer::stopTimer(11);
		Timer::createTimer(11, 10.0f);
	}

	if (Timer::hasTimerFinished(11) && stage < 5) //if it is time for the next stage and the stage is less than 6
	{
		stage++;//next stage
		maths::Mat43 m = maths::Mat43::kIdentity;
		maths::Vec3 colour;
		switch (stage)//set up the next stage
		{
		case 1:

			cues[0]->updateColour(maths::Vec3(1.0f, 1.0f, 0.0f));
			cues[1]->updateColour(maths::Vec3(1.0f, 1.0f, 0.0f));

			lockedZ = handPos.z; //sets the current hand z to the locked z

			Timer::createTimer(11, 5.0f); //starts the 5 second timer for this stage
			break;
		case 2:
			cues[0]->updateColour(maths::Vec3(0.0f, 1.0f, 0.0f));
			cues[1]->updateColour(maths::Vec3(0.0f, 1.0f, 0.0f));

			lockedX = handPos.x;// sets the current hand x to the locked x
			lockedZ = handPos.z;
			Timer::createTimer(19, 10.0f); //sets ups the timer for the kinect input maths
			Timer::createTimer(11, 10.0f); //sets up the backup timer so that it auto throws after 10 seconds

			break;
		case 3:
			Timer::createTimer(11, 10.0f); //set time to reset the game after 10 seconds
			jack->updateXVelocity(10.0f); //auto throw the ball
			break;
		case 4:
			//reset the game
			stage = 0;
			resetPositions();
			Timer::stopTimer(11);
			Timer::createTimer(11, 10.0f);
			break;
		}
	}
	if (stage == 2 && handPos.x > lockedX + 3.0f) //check if the current stage is throw and if so check if the hand has moved far enougth to be a throw
	{
		float testTime = Timer::stopTimer(19); //get the time it took to throw
		if ((Physics::kinectInputVelocity(3.0f, testTime)) < jack->getThrow()) //check if the new velocity is greater than the max throw
		{
			jack->updateXVelocity((Physics::kinectInputVelocity(3.0f, testTime))*10.0f); // if not times it by 10 and use it as the throw
		}
		else
		{
			jack->updateXVelocity(jack->getThrow()*10.0f); // if so set the throw to the max
		}
		jack->updateZVelocity((Physics::kinectInputVelocity((handPos.z - lockedZ), testTime))*10.0f);
		//set the game to reset in 10 seconds
		Timer::stopTimer(11);
		Timer::createTimer(11, 10.0f);
	}

	//ball ball collision check tests
	if (Physics::collisionCheck(red, blue, dt))
	{
		//new velocities
		Physics::newCollisionVelocities(red, blue);
	}
	if (Physics::collisionCheck(red, jack, dt))
	{
		//new velocities
		Physics::newCollisionVelocities(red, jack);
	}
	if (Physics::collisionCheck(blue, jack, dt))
	{
		//new velocities
		Physics::newCollisionVelocities(blue, jack);
	}
	if (Physics::collisionCheck(red, blue, dt))
	{
		//new velocities
		Physics::newCollisionVelocities(red, blue);
	}
	//wall wall collision tests
	if (Physics::collisionCheck(jack, endWall, dt))
	{
		//new velocities
		Physics::newCollisionVelocities(jack, endWall);
	}
	if (Physics::collisionCheck(jack, sideWall1, dt))
	{
		//new velocities
		Physics::newCollisionVelocities(jack, sideWall1);
	}
	if (Physics::collisionCheck(jack, sideWall2, dt))
	{
		//new velocities
		Physics::newCollisionVelocities(jack, sideWall2);
	}
	if (Physics::collisionCheck(blue, endWall, dt))
	{
		//new velocities
		Physics::newCollisionVelocities(blue, endWall);
	}
	if (Physics::collisionCheck(blue, sideWall1, dt))
	{
		//new velocities
		Physics::newCollisionVelocities(blue, sideWall1);
	}
	if (Physics::collisionCheck(blue, sideWall2, dt))
	{
		//new velocities
		Physics::newCollisionVelocities(blue, sideWall2);
	}
	if (Physics::collisionCheck(blue, endWall, dt))
	{
		//new velocities
		Physics::newCollisionVelocities(red, endWall);
	}
	if (Physics::collisionCheck(red, sideWall1, dt))
	{
		//new velocities
		Physics::newCollisionVelocities(red, sideWall1);
	}
	if (Physics::collisionCheck(red, sideWall2, dt))
	{
		//new velocities
		Physics::newCollisionVelocities(red, sideWall2);
	}

	//move balls
	red->changePosition(cgg::Vec3(red->getXVelocity() * dt, 0, red->getZVelocity() * dt));
	blue->changePosition(cgg::Vec3(blue->getXVelocity() * dt, 0, blue->getZVelocity() * dt));
	jack->changePosition(cgg::Vec3(jack->getXVelocity() * dt, 0, jack->getZVelocity() * dt));

	//friction
	Physics::applyFriction(red);
	Physics::applyFriction(blue);
	Physics::applyFriction(jack);

	//update the closest ball
	if (stage > 2)
	{
		if (getClosestBallType() == 'B')
		{
			closestBowlType[0]->updateColour(maths::Vec3(0.0f, 0.0f, 1.0f));
			closestBowlType[1]->updateColour(maths::Vec3(0.0f, 0.0f, 1.0f));
		}
		else
		{
			closestBowlType[0]->updateColour(maths::Vec3(1.0f, 0.0f, 0.0f));
			closestBowlType[1]->updateColour(maths::Vec3(1.0f, 0.0f, 0.0f));
		}
	}
}

void Game::loadWorld()
{
	// generate a red sphere
	maths::Mat43 m = maths::Mat43::kIdentity;
	m.w.x = 0;
	m.w.z = 0;
	m.w.y = -2;

	maths::Vec3 colour;
	colour.x = 1;
	colour.y = 0;
	colour.z = 0;
	red = new Bowl(m, colour, 1);

	// generate a blue sphere
	m.w.x = 8;
	m.w.z = 0;
	m.w.y = -2;

	colour.x = 0;
	colour.y = 0;
	colour.z = 1;
	blue = new Bowl(m, colour, 1);

	// generate a jack
	m.w.x = -20;
	m.w.z = 0;
	m.w.y = 0;

	colour.x = 1;
	colour.y = 1;
	colour.z = 0;
	jack = new Jack(m, colour, 0.5);

	// generate a ball to show what team is closest
	m.w.x = -19.5;
	m.w.z = -5.5;
	m.w.y = 0;

	colour.x = 1;
	colour.y = 1;
	colour.z = 1;
	closestBowlType[0] = new Ball(m, colour, 0.5);

	// generate a ball to show what team is closest
	m.w.x = -19.5;
	m.w.z = 5.5;
	m.w.y = 0;

	colour.x = 1;
	colour.y = 1;
	colour.z = 1;
	closestBowlType[1] = new Ball(m, colour, 0.5);

	// generate a cue
	m.w.x = -19.5f;
	m.w.z = -5.5f;
	m.w.y = -0.5f;

	colour.x = 1;
	colour.y = 0;
	colour.z = 0;
	cues[0] = new Box(m, colour, 1, false);

	// generate a cue
	m.w.x = -19.5f;
	m.w.z = 5.5f;
	m.w.y = -0.5f;

	colour.x = 1;
	colour.y = 0;
	colour.z = 0;
	cues[1] = new Box(m, colour, 1, false);

	// generate ground
	m.w.x = 0;
	m.w.z = 0;
	m.w.y = -4;

	colour.x = 0;
	colour.y = 1;
	colour.z = 0;
	ground = new Box(m, colour, cgg::Vec3(40, 2, 10), false);

	// generate endWall
	m.w.x = 20.5f;
	m.w.z = 0;
	m.w.y = -3;

	colour.x = 1;
	colour.y = 1;
	colour.z = 1;
	endWall = new Box(m, colour, cgg::Vec3(1, 4, 12), true);

	// generate sideWall1
	m.w.x = 0;
	m.w.z = -5.5f;
	m.w.y = -3;

	colour.x = 1;
	colour.y = 1;
	colour.z = 1;
	sideWall1 = new Box(m, colour, cgg::Vec3(40, 4, 1), false);

	// generate sideWall2
	m.w.x = 0;
	m.w.z = 5.5f;
	m.w.y = -3;

	colour.x = 1;
	colour.y = 1;
	colour.z = 1;
	sideWall2 = new Box(m, colour, cgg::Vec3(40, 4, 1), false);
}

void Game::render(gl::Primitives* g_prims)
{
	closestBowlType[0]->render(g_prims);
	closestBowlType[1]->render(g_prims);
	cues[0]->render(g_prims);
	cues[1]->render(g_prims);
	jack->render(g_prims);
	red->render(g_prims);
	blue->render(g_prims);
	ground->render(g_prims);
	endWall->render(g_prims);
	sideWall1->render(g_prims);
	sideWall2->render(g_prims);
}

void Game::startGame()
{
	int coinFlip = randomNumber(0, 1);
	if (coinFlip)
	{
		currentTurn = BluePlayer;
		cgg::logi("Blue Player wins the coin toss, they go first");
	}
	else
	{
		currentTurn = RedPlayer;
		cgg::logi("Red Player wins the coin toss, they go first");
	}

	//hand = new Jack(handPos, jackColour, jackRadius);
	
}

void Game::playerTurnStart()
{
	//Matrix handling & Kinect Pos Handling
	cgg::Mat43 kinect;

	Bowl* bowl;
	//Need to handle Jack special case
	switch (currentTurn)
	{
	case RedPlayer:
		bowl = new Bowl(kinect, redColour, bowlRadius);

		redBowls.push_back(bowl);
		break;

	case BluePlayer:
		bowl = new Bowl(kinect, blueColour, bowlRadius);
		
		blueBowls.push_back(bowl);
		break;

	default:
		cgg::loge("Game Logic: Invalid player name enum passed to throwBowl()");
		break;
	}
}

void Game::throwBowl()
{
	
}

//test the closest ball
char Game::getClosestBallType()
{
	char closestType;
	float redDistance = Physics::distanceBetweenTospheres(red->getPosition(), jack->getPosition());
	float blueDistance = Physics::distanceBetweenTospheres(blue->getPosition(), jack->getPosition());
	if (redDistance < blueDistance)
	{
		return 'R';
	}
	else
	{
		return 'B';
	}
}

//reset the positions
void Game::resetPositions()
{
	red->updatePosition(maths::Vec3(0.0f, -2.0f, 0.0f));
	blue->updatePosition(maths::Vec3(8.0f, -2.0f, 0.0f));
	jack->updatePosition(maths::Vec3(-20.0f, 0.0f, 0.0f));
	cues[0]->updateColour(maths::Vec3(1.0f, 0.0f, 0.0f));
	cues[1]->updateColour(maths::Vec3(1.0f, 0.0f, 0.0f));
	closestBowlType[0]->updateColour(maths::Vec3(1.0f, 1.0f, 1.0f));
	closestBowlType[1]->updateColour(maths::Vec3(1.0f, 1.0f, 1.0f));
}