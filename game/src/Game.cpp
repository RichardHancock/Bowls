#include "Game.h"

Game::Game(bool sitMode, TrackingPoint hand)
{
	timerCurrentTime = 0;
	timerMaxTime = -1;
	srand(time(NULL));
	kinectSensor = new KinectInput(sitMode, hand);
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
	// If the timer is set
	if (timerMaxTime >= 0.0f)
	{
		//increment currentTime
		timerCurrentTime += (1 * dt);
	}

	kinectSensor->update();
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

bool Game::timerExpired()
{
	if (timerCurrentTime < timerMaxTime)
	{
		return false;
	}
	else 
	{
		// reset timer and return true
		timerMaxTime = -1;
		timerCurrentTime = 0;
		return true;
	}
}