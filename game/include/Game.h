#include <time.h>
#include "cgg/cgg.h"

#include "Kinect.h"
#include "Timer.h"

#include "Bowl.h"
#include "Jack.h"
#include "Physics.h"
#include "Audio.h"

/// an eNum to identify the current players turn
enum PlayerName
{
	RedPlayer,
	BluePlayer
};

/**
@brief the Game object where all of the functions of the game is stored.
*/
class Game
{
public:
	/**
	constructor
	@param bool sitmode, if the Kinect sensor should look for sitting skeletons, if not its a standing skeleton
	@param TrackingPoint which hand the Kinect should track.
	*/
	Game(bool, TrackingPoint);

	/**
	destructor
	*/
	~Game();

	/**
	The update function that is accessed int the update function in main
	Updates all of the Game information

	@param float the deltaTime
	@param cgg::MayaCamera a reference to the camera
	*/
	void update(float dt, cgg::MayaCamera &g_camera);

	/**
	Accessed in the load assets in main and loads the initial data for the game.
	*/
	void loadWorld();

	/**
	This is where all of the entities are rendered. This is accessed by the update function in main.

	@param gl::Primitives* a pointer to the primitives
	*/
	void render(gl::Primitives*);

	/**
	initialises the game
	*/
	void startGame();
private:
	int stage = 0; ///the current cue stage
	float lockedZ = 0.0f; /// the z position to be used with input maths
	float lockedX = 0.0f; /// the x position to be used with input maths
	
	///entities
	std::vector<Bowl*> redBowls;
	std::vector<Bowl*> blueBowls;
	std::vector<Ball*> scoreDisplay;
	Box * cues[2];
	Ball * cueHat[2];
	Jack * jack;
	PlayerName currentTurn;
	Ball * hand;
	Box* ground;
	Box* endWall;
	Box* sideWall1;
	Box* sideWall2;

	//Audio data
	Audio* music;
	Audio* aimCue;
	Audio* armCue;
	Audio* throwCue;

	///kinect data
	KinectInput* kinectSensor;
	
	///scores
	int blueScore;
	int redScore;

	///colours
	const cgg::Vec3 redColour = { 1, 0, 0 };
	const cgg::Vec3 blueColour = { 0, 0, 1 };
	const cgg::Vec3 yellowColour = { 1, 1, 0 };
	const cgg::Vec3 greenColour = { 0, 1, 0 };
	const cgg::Vec3 whiteColour = { 1, 1, 1 };

	///constants
	const float aimStage = 10.0f;
	const float pullBackStage = 5.0f;
	const float throwStage = 10.0f;
	const float autoThrowTime = 10.0f;
	const float delayBeforeNextTurn = 10.0f;
	const float bowlRadius = 0.4;
	const float jackRadius = 0.3;

	///the offset between the ball in hand and the floor, initially the jack
	float handOffset = -1.7f;

	/**
	The processes that needs to happen at the start of the turn
	*/
	void playerTurnStart();

	/**
	Looks for the closest ball to the jack and returns its type as a R or B

	@returns char the type of the ball
	*/
	char getClosestBallType();

	/**
	Resets the positions of things such as the camera every turn

	@param cgg::MayaCamera a refernce to the camera
	*/
	void resetPositions(cgg::MayaCamera &g_camera);

	/**
	Runs the tests for the ball wall collisions

	@param Ball* a pointer to the ball to test
	@param float the deltaTime
	*/
	void ballWallCollisionTests(Ball *, float);

	/**
	The processes that needs to happen at end of the game
	*/
	void gameOver();

	/**
	Displays the score in the form of the score coloured balls on the side of the screen
	*/
	void displayScore();

	/**
	Generates a random number between 2 values

	@param int the minimum value
	@param int the maximum value
	*/
	inline int randomNumber(int minimum, int maximum)
	{
		return rand() % (maximum - minimum + 1) + minimum;
	}

};