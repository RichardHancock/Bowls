#include "Game.h"

Game::Game(bool sitMode, TrackingPoint hand)
{
	srand((unsigned int)time(NULL));
	kinectSensor = new KinectInput(sitMode, hand);
	stage = 0; //the current cue stage
	lockedZ = 0.0f; // the z position to be used with input maths
	lockedX = 0.0f;
	// TODO: Might need a conditional to check here for errors
	kinectSensor->startTracking();
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
	delete hand;
	delete ground;
	delete endWall;
	delete sideWall1;
	delete sideWall2;
	delete cues[0];
	delete cues[1];
	delete closestBowlType[0];
	delete closestBowlType[1];
}

void Game::update(float dt, cgg::MayaCamera &g_camera)
{
	kinectSensor->update();
	cgg::Vec3 handPos = kinectSensor->getHandPos();

	switch (stage)//moves the kinect on screen differently depending on the stage
	{
	case 0:
		// Kinect Input - moves in z axis
		hand->updatePosition(cgg::Vec3(-17, -1, 0) + cgg::Vec3(-5, handOffset, handPos.z));
		break;
	case 1:
	case 2:
		// Kinect Input - moves in x axis
		hand->updatePosition(cgg::Vec3(-17, -1, 0) + cgg::Vec3(handPos.x, handOffset, lockedZ));
		break;
	}

	if (stage == 4)
	{
		g_camera.setCentreOfInterest(hand->getPosition());
	}

	if (cgg::isKeyPressed(cgg::kKeyEscape))
	{
		exit(0);
	}
	if (cgg::isKeyPressed(cgg::kKeyEnter))
	{
		//reset the game
		stage = 0;
		resetPositions(g_camera);
		jack->updatePosition(maths::Vec3(-20.0f, 0.0f, 0.0f));
		closestBowlType[0]->updateColour(maths::Vec3(1.0f, 1.0f, 1.0f));
		closestBowlType[1]->updateColour(maths::Vec3(1.0f, 1.0f, 1.0f));
		handOffset = -1.5f;
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
			stage = 4;
			Timer::createTimer(11, 10.0f); //set time to reset the game after 10 seconds
			hand->updateXVelocity(10.0f); //auto throw the ball
			break;
		case 5:
			//check if game end
			if (blueBowls.size() == 4 && redBowls.size() == 4)
			{
				stage == 6;//end game stage
				resetPositions(g_camera);
				hand == 0;
			}
			else
			{
				//change the ball
				stage = 0;
				resetPositions(g_camera);
				Timer::stopTimer(11);
				Timer::createTimer(11, 10.0f);
				if (currentTurn == BluePlayer)
				{
					redBowls.push_back(new Bowl(maths::Vec3(-20.0f, 0.0f, 0.0f), redColour, bowlRadius));
					hand = redBowls[redBowls.size() - 1];
					currentTurn = RedPlayer;
					handOffset = -1.6f;
				}
				else if (currentTurn == RedPlayer)
				{
					blueBowls.push_back(new Bowl(maths::Vec3(-20.0f, 0.0f, 0.0f), blueColour, bowlRadius));
					hand = blueBowls[blueBowls.size() - 1];
					currentTurn = BluePlayer;
					handOffset = -1.6f;
				}
				break;
			}
		}
	}
	if (stage == 2 && handPos.x > lockedX + 3.0f) //check if the current stage is throw and if so check if the hand has moved far enough to be a throw
	{
		float testTime = Timer::stopTimer(19); //get the time it took to throw
		if ((Physics::kinectInputVelocity(3.0f, testTime)) < jack->getThrow()) //check if the new velocity is greater than the max throw
		{
			hand->updateXVelocity((Physics::kinectInputVelocity(3.0f, testTime))*10.0f); // if not times it by 10 and use it as the throw
		}
		else
		{
			hand->updateXVelocity(jack->getThrow()*10.0f); // if so set the throw to the max
		}
		hand->updateZVelocity((Physics::kinectInputVelocity((handPos.z - lockedZ), testTime))*10.0f);
		stage = 4;//set the game to reset in 10 seconds
		Timer::stopTimer(11);
		Timer::createTimer(11, 10.0f);
	}
	
	//red ball wall collision tests
	for (int i = 0; i < redBowls.size(); i++)
	{
		ballWallCollisionTests(redBowls[i], dt);
	}
	//blue ball wall collision tests
	for (int i = 0; i < blueBowls.size(); i++)
	{
		ballWallCollisionTests(blueBowls[i], dt);
	}
	//jack ball wall collision tests
	ballWallCollisionTests(jack, dt);
	
	//ball ball collision tests
	std::vector<Ball*> allBalls;
	allBalls.reserve(redBowls.size() + blueBowls.size() + 1);
	allBalls.insert(allBalls.end(), redBowls.begin(), redBowls.end());
	allBalls.insert(allBalls.end(), blueBowls.begin(), blueBowls.end());
	allBalls.push_back(jack);
	for (int i = 0; i < allBalls.size(); i++)
	{
		for (int j = 0; j < allBalls.size(); j++)
		{
			//ball ball collision check tests
			if (i != j && Physics::collisionCheck(allBalls[i], allBalls[j], dt))
			{
				//new velocities
				Physics::newCollisionVelocities(allBalls[i], allBalls[j]);
			}
		}
	}
	allBalls.clear();

	//move balls & apply friction
	for (int i = 0; i < redBowls.size(); i++)
	{
		redBowls[i]->changePosition(cgg::Vec3(redBowls[i]->getXVelocity() * dt, 0.0f, redBowls[i]->getZVelocity() * dt));
		Physics::applyFriction(redBowls[i]);
	}
	for (int i = 0; i < blueBowls.size(); i++)
	{
		blueBowls[i]->changePosition(cgg::Vec3(blueBowls[i]->getXVelocity() * dt, 0.0f, blueBowls[i]->getZVelocity() * dt));
		Physics::applyFriction(blueBowls[i]);
	}
	jack->changePosition(cgg::Vec3(jack->getXVelocity() * dt, 0, jack->getZVelocity() * dt));
	Physics::applyFriction(jack);

	//update the closest ball type
	if (stage > 2 && redBowls.size() > 0 || stage > 2 &&  blueBowls.size() > 0)
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
	// generate the balls to show what team is closest
	closestBowlType[0] = new Ball(maths::Vec3(-19.5f, 0.0f, -5.5f), whiteColour, 0.5f);
	closestBowlType[1] = new Ball(maths::Vec3(-19.5f, 0.0f, 5.5f), whiteColour, 0.5f);

	// generate the cues
	cues[0] = new Box(maths::Vec3(-19.5f, -0.5f, -5.5f), redColour, maths::Vec3(1.0f), false);
	cues[1] = new Box(maths::Vec3(-19.5f, -0.5f, 5.5f), redColour, maths::Vec3(1.0f), false);

	// generate ground
	ground = new Box(maths::Vec3(0.0f, -4.0f, 0.0f), greenColour, cgg::Vec3(40, 2, 10), false);

	// generate endWall
	endWall = new Box(maths::Vec3(20.5f, -3.0f, 0.0f), whiteColour, cgg::Vec3(1, 4, 12), true);

	// generate sideWall1
	sideWall1 = new Box(maths::Vec3(0.0f, -3.0f, -5.5f), whiteColour, cgg::Vec3(40, 4, 1), false);

	// generate sideWall2
	sideWall2 = new Box(maths::Vec3(0.0f, -3.0f, 5.5f), whiteColour, cgg::Vec3(40, 4, 1), false);
}

void Game::render(gl::Primitives* g_prims)
{
	closestBowlType[0]->render(g_prims);
	closestBowlType[1]->render(g_prims);
	cues[0]->render(g_prims);
	cues[1]->render(g_prims);
	jack->render(g_prims);
	for (int i = 0; i < redBowls.size(); i++)
	{
		redBowls[i]->render(g_prims);
	}
	for (int i = 0; i < blueBowls.size(); i++)
	{
		blueBowls[i]->render(g_prims);
	}	
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

	// generate a jack
	jack = new Jack(maths::Vec3(-20.0f, 0.0f, 0.0f), yellowColour, jackRadius);

	//set the jack to the players hand
	hand = jack;
	
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

//test the closest ball
char Game::getClosestBallType()
{
	float redDistance = 1000.0f;//initialise to huge numbers
	float blueDistance = 1000.0f;//initialise to huge numbers
	for (int i = 0; i < redBowls.size(); i++)
	{
		if (redDistance > Physics::distanceBetweenTwoSpheres(redBowls[i]->getPosition(), jack->getPosition()))
		{
			redDistance = Physics::distanceBetweenTwoSpheres(redBowls[i]->getPosition(), jack->getPosition());
			cgg::logi(std::to_string(redDistance).c_str());
		}
	}
	cgg::logi("blue");
	for (int i = 0; i < blueBowls.size(); i++)
	{
		if (blueDistance > Physics::distanceBetweenTwoSpheres(blueBowls[i]->getPosition(), jack->getPosition()))
		{
			blueDistance = Physics::distanceBetweenTwoSpheres(blueBowls[i]->getPosition(), jack->getPosition());
			cgg::logi(std::to_string(blueDistance).c_str());
		}
	}

	if (blueBowls.size() <= 0)
	{
		blueDistance = 1000.0f;
	}

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
void Game::resetPositions(cgg::MayaCamera &g_camera)
{
	cues[0]->updateColour(maths::Vec3(1.0f, 0.0f, 0.0f));
	cues[1]->updateColour(maths::Vec3(1.0f, 0.0f, 0.0f));
	g_camera.setCentreOfInterest(cgg::Vec3(-20, 0, 0));
}

//ball wall collision tests
void Game::ballWallCollisionTests(Ball * ball, float dt)
{
	//end wall test
	if (Physics::collisionCheck(ball, endWall, dt))
	{
		//new velocities
		Physics::newCollisionVelocities(ball, endWall);
	}
	//side wall 1 test
	if (Physics::collisionCheck(ball, sideWall1, dt))
	{
		//new velocities
		Physics::newCollisionVelocities(ball, sideWall1);
	}
	//side wall 2 test
	if (Physics::collisionCheck(ball, sideWall2, dt))
	{
		//new velocities
		Physics::newCollisionVelocities(ball, sideWall2);
	}
}