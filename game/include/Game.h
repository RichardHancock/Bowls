#include <time.h>
#include "cgg/cgg.h"

#include "Kinect.h"
#include "Timer.h"

#include "Bowl.h"
#include "Jack.h"

enum PlayerName
{
	RedPlayer,
	BluePlayer
};

class Game
{
public:
	Game(bool, TrackingPoint);
	~Game();

	void update(float dt);
	void startGame();
private:

	std::vector<Bowl*> redBowls;
	std::vector<Bowl*> blueBowls;
	Jack* jack;
	PlayerName currentTurn;
	Ball* hand;

	KinectInput* kinectSensor;
	
	const cgg::Vec3 redColour = { 1, 0, 0 };
	const cgg::Vec3 blueColour = { 0, 0, 1 };
	const cgg::Vec3 jackColour = { 1, 1, 0 };

	const float bowlRadius = 1;
	const float jackRadius = 0.5;

	void playerTurnStart();
	void throwBowl();

	inline int randomNumber(int minimum, int maximum)
	{
		return rand() % (maximum - minimum + 1) + minimum;
	}

};