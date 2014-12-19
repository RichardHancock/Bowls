#include "Timer.h"

std::vector<TimerStruct> Timer::timers;
const float Timer::TIME_INTERVAL = 1.0f;
int Timer::timerCount = 0;


Timer::Timer() {}

void Timer::update(float dt)
{
	for (int i = 0; i < timerCount; i++)
	{
		timers[i].currentTime += (TIME_INTERVAL * dt);
	}
}

bool Timer::createTimer(int id, float duration)
{
	if (searchForIDPos(id) != -1)
	{
		cgg::logw("Timer id specified already exists");
		return false;
	}
	
	//Increment the amount of timers
	timerCount++;

	//Create the Timer
	TimerStruct timer = { id, 0.0f, duration };

	//Push it into the timers vector
	timers.push_back(timer);

	return true;
}

float Timer::stopTimer(int id)
{
	int timerPos = searchForIDPos(id);
	if (timerPos == -1) { return -1.0f; }
	
	float lastCurrentTime = timers[timerPos].currentTime;

	timerCount--;
	timers.erase(timers.begin() + timerPos);

	return lastCurrentTime;
}

bool Timer::hasTimerFinished(int id)
{
	int timerPos = searchForIDPos(id);
	if (timerPos < 0) 
	{ 
		cgg::logw("Unknown timer was asked if finished.");
		return false; 
	}
	else
	{
		TimerStruct timer = timers[timerPos];
		if(timer.currentTime >= timer.duration)
		{
			--timerCount;
			timers.erase(timers.begin() + timerPos);

			return true;
		}
	}
	return false;
}

int Timer::searchForIDPos(int id)
{
	for (int i = 0; i < timerCount; i++)
	{
		if(timers[i].id == id)
		{
			return i;
		}
	}

	return -1;
}

