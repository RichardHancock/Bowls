#pragma once

#include <vector>

#include "cgg/cgg.h"

struct TimerStruct
{
	int id;
	float currentTime;
	float duration;
};

class Timer
{
public:

	static void update(float dt);

	static bool createTimer(int id, float duration);
	static float stopTimer(int id);

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