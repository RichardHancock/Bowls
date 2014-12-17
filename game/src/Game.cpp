#include "Game.h"

Game::Game()
{
	timerCurrentTime = 0;
	timerMaxTime = -1;
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
}

void Game::playerTurn()
{
	//Matrix handling & Kinect Pos Handling
	cgg::Mat43 kinect;



	//Need to handle Jack special case
	switch (currentTurn)
	{
	case RedPlayer:
		Bowl* redBowl = new Bowl(kinect, redColour, bowlRadius);

		redBowls.push_back(redBowl);
		break;

	case BluePlayer:
		Bowl* blueBowl = new Bowl(kinect, blueColour, bowlRadius);
		
		blueBowls.push_back(blueBowl);
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