#include "Game.h"

Game::Game(bool sitMode, TrackingPoint hand)
{
	srand((unsigned int)time(NULL));
	kinectSensor = new KinectInput(sitMode, hand);
	stage = 0; //the current cue stage
	lockedZ = 0.0f; // the z position to be used with input maths
	lockedX = 0.0f;

	blueScore = 0;
	redScore = 0;
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

	for (int i = 0; i < scoreDisplay.size(); i++)
	{
		delete scoreDisplay[i];
	}

	delete jack;
	delete hand;
	delete ground;
	delete endWall;
	delete sideWall1;
	delete sideWall2;
	delete cues[0];
	delete cues[1];
	delete cueHat[0];
	delete cueHat[1];
	delete kinectSensor;
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
		handOffset = -1.5f;
		hand = jack;
		for (int i = 0; i < redBowls.size(); i++)
		{
			delete redBowls[i];
		}

		for (int i = 0; i < blueBowls.size(); i++)
		{
			delete blueBowls[i];
		}

		for (int i = 0; i < scoreDisplay.size(); i++)
		{
			delete scoreDisplay[i];
		}
		redBowls.clear();
		blueBowls.clear();
		scoreDisplay.clear();
		blueScore = 0;
		redScore = 0;
		Timer::stopTimer(11);
		Timer::createTimer(11, aimStage);
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
			Timer::createTimer(11, pullBackStage); //starts the 5 second timer for this stage
			break;
		case 2:
			cues[0]->updateColour(maths::Vec3(0.0f, 1.0f, 0.0f));
			cues[1]->updateColour(maths::Vec3(0.0f, 1.0f, 0.0f));
			lockedX = handPos.x;// sets the current hand x to the locked x
			lockedZ = handPos.z;
			Timer::createTimer(19, throwStage); //sets ups the timer for the kinect input maths
			Timer::createTimer(11, autoThrowTime); //sets up the backup timer so that it auto throws after 10 seconds
			break;
		case 3:
			stage = 4;
			Timer::createTimer(11, delayBeforeNextTurn); //set time to reset the game after 10 seconds
			hand->updateXVelocity(10.0f); //auto throw the ball
			break;
		case 5:
			//check if game end
			if (blueBowls.size() == 4 && redBowls.size() == 4)
			{
				stage = 6;//end game stage
				resetPositions(g_camera);
				hand = NULL;
				gameOver();
			}
			else
			{
				//change the ball
				stage = 0;
				resetPositions(g_camera);
				Timer::stopTimer(11);
				Timer::createTimer(11, aimStage);
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
				
			}
			break;
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
		Timer::createTimer(11, delayBeforeNextTurn);
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
}

void Game::loadWorld()
{
	// generate the cues each cue has a hat, just for looks :-)
	cues[0] = new Box(maths::Vec3(-19.5f, -0.5f, -5.5f), redColour, maths::Vec3(1.0f), false);
	cueHat[0] = new Ball(maths::Vec3(-19.5f, 0.0f, -5.5f), whiteColour, 0.5f);
	cues[1] = new Box(maths::Vec3(-19.5f, -0.5f, 5.5f), redColour, maths::Vec3(1.0f), false);
	cueHat[1] = new Ball(maths::Vec3(-19.5f, 0.0f, 5.5f), whiteColour, 0.5f);

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
	cues[0]->render(g_prims);
	cueHat[0]->render(g_prims);
	cues[1]->render(g_prims);
	cueHat[1]->render(g_prims);
	jack->render(g_prims);
	for (int i = 0; i < redBowls.size(); i++)
	{
		redBowls[i]->render(g_prims);
	}
	for (int i = 0; i < blueBowls.size(); i++)
	{
		blueBowls[i]->render(g_prims);
	}

	//Score Balls
	for (int i = 0; i < scoreDisplay.size(); i++)
	{
		scoreDisplay[i]->render(g_prims);
	}

	//World
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

	// setup initial cue timer
	Timer::createTimer(11, aimStage);
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

void Game::gameOver()
{
	cgg::Vec3 jackPos = jack->getPosition();

	std::vector<float> redBowlsDistance;
	std::vector<float> blueBowlsDistance;

	for (int r = 0; r < (int)redBowls.size(); r++)
	{
		cgg::Vec3 rPos = redBowls[r]->getPosition();
		float distance = Physics::distanceBetweenTwoSpheres(rPos, jackPos);
		redBowlsDistance.push_back(distance);
	}
	for (int b = 0; b < (int)blueBowls.size(); b++)
	{
		cgg::Vec3 bPos = blueBowls[b]->getPosition();
		float distance = Physics::distanceBetweenTwoSpheres(bPos, jackPos);
		blueBowlsDistance.push_back(distance);
	}

	std::sort(redBowlsDistance.begin(), redBowlsDistance.end());
	std::sort(blueBowlsDistance.begin(), blueBowlsDistance.end());

	PlayerName winner;

	winner = (blueBowlsDistance[0] < redBowlsDistance[0] ? BluePlayer : RedPlayer);
	std::string winnerName = (winner == RedPlayer ? "Red Player" : "Blue Player");
	std::string message = "Game Over - " + winnerName + " Wins!";
	cgg::logi(message.c_str());

	//Calculate score
	int score = 1;

	if (winner == RedPlayer)
	{
		//Start one later as we already know at least one point was achieved.
		for (int i = 1; i < redBowlsDistance.size(); i++)
		{
			if (redBowlsDistance[i] < blueBowlsDistance[0])
			{
				score++;
			}
			else
			{
				break;
			}
		}

		redScore = score;
	}
	else
	{
		//Start one later as we already know at least one point was achieved.
		for (int i = 1; i < blueBowlsDistance.size(); i++)
		{
			if (blueBowlsDistance[i] < redBowlsDistance[0])
			{
				score++;
			}
			else
			{
				break;
			}
		}
		blueScore = score;
	}

	displayScore();
}

void Game::displayScore()
{
	float gapY = 1.0f;
	float startY = -5.0f;
	float bothX = -20.0f;
	float redZ = -7.0f;
	float blueZ = 7.0f;

	for (int i = 0; i < (int)redScore; i++)
	{
		cgg::Vec3 pos = { bothX, startY + (gapY * i), redZ };
		Ball* scoreBall = new Ball(pos, redColour, bowlRadius);
		scoreDisplay.push_back(scoreBall);
	}

	for (int i = 0; i < (int)blueScore; i++)
	{
		cgg::Vec3 pos = { bothX, startY + (gapY * i), blueZ };
		Ball* scoreBall = new Ball(pos, blueColour, bowlRadius);
		scoreDisplay.push_back(scoreBall);
	}
}