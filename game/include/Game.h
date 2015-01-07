#include <time.h>
#include "cgg/cgg.h"

#include "Kinect.h"
#include "Timer.h"

#include "Bowl.h"
#include "Jack.h"
#include "Physics.h"

enum PlayerName
{
	RedPlayer,
	BluePlayer
};

class Game
{
public:
	/**
	@param bool sitmode, if the Kinect sensor should look for sitting skeletons, if not its a standing skeleton
	@param TrackingPoint which hand the Kinect should track.
	*/
	Game(bool, TrackingPoint);
	~Game();

	void update(float dt, cgg::MayaCamera &g_camera);
	void loadWorld();
	void render(gl::Primitives*);
	void startGame();
private:
	int stage = 0; //the current cue stage
	float lockedZ = 0.0f; // the z position to be used with input maths
	float lockedX = 0.0f; /// the x position to be used with input maths

	std::vector<Bowl*> redBowls;
	std::vector<Bowl*> blueBowls;
	std::vector<Ball*> scoreDisplay;
	Box * cues[2];
	Ball * closestBowlType[2];
	Jack * jack;
	PlayerName currentTurn;
	Ball * hand;
	Box* ground;
	Box* endWall;
	Box* sideWall1;
	Box* sideWall2;

	KinectInput* kinectSensor;
	
	int blueScore;
	int redScore;

	//colours
	const cgg::Vec3 redColour = { 1, 0, 0 };
	const cgg::Vec3 blueColour = { 0, 0, 1 };
	const cgg::Vec3 yellowColour = { 1, 1, 0 };
	const cgg::Vec3 greenColour = { 0, 1, 0 };
	const cgg::Vec3 whiteColour = { 1, 1, 1 };

	//Original times are the ones on the left 
	///@todo Make first timer affected by these
	const float aimStage = 10.0f - 8.0f;
	const float pullBackStage = 5.0f - 3.0f;
	const float throwStage = 10.0f - 6.0f;
	const float autoThrowTime = 10.0f - 6.0f;
	const float delayBeforeNextTurn = 10.0f - 6.0f;
	const float bowlRadius = 0.4;
	const float jackRadius = 0.3;
	float handOffset = -1.7f;

	void playerTurnStart();
	char getClosestBallType();
	void resetPositions(cgg::MayaCamera &g_camera);
	void ballWallCollisionTests(Ball *, float);
	void gameOver();
	void displayScore();

	inline int randomNumber(int minimum, int maximum)
	{
		return rand() % (maximum - minimum + 1) + minimum;
	}

};