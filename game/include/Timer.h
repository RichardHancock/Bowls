#pragma once

#include <vector>

#include "cgg/cgg.h"

///a structure for the timer
struct TimerStruct
{
	int id;
	float currentTime;
	float duration;
};

/**
@brief creates a timer for use in game
*/
class Timer
{
public:
	/**
	upadates the time in the timer

	@param float the deltaTime
	*/
	static void update(float dt);

	/**
	Creates a new timer with a unique id and a duration

	@param int the id
	@param float the duration
	@returns bool false is id is not unique
	*/
	static bool createTimer(int id, float duration);

	/**
	Stops the time and returns the time taken

	@param int the id
	@returns float the time taken
	*/
	static float stopTimer(int id);

	/**
	Checks to see if the timer has finished

	@param int the id
	@returns bool if the timer has ended
	*/
	static bool hasTimerFinished(int id);

private:
	//No one should call this so it is private
	Timer();

	static const float TIME_INTERVAL;

	/**
	@brief Stores the Timers

	1- id
	2- currentTime
	3- duration
	*/
	static std::vector<TimerStruct> timers;
	static int timerCount;
	static int searchForIDPos(int id);
};