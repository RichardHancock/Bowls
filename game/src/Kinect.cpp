#include "Kinect.h"
#include <iostream>

KinectInput::KinectInput(bool sitMode, TrackingPoint hand) 
	: sitMode(sitMode)
{
	kinectSensor = NULL;
	nextSkeletonUpdate = NULL;

	switchHand(hand);
	handPos = { 0, 0, 0 };

	initSensor();
}

KinectInput::~KinectInput()
{
	if (kinectSensor) { kinectSensor->NuiShutdown(); }

	if (nextSkeletonUpdate && (nextSkeletonUpdate != INVALID_HANDLE_VALUE))
	{
		CloseHandle(nextSkeletonUpdate);
	}
}

bool KinectInput::initSensor()
{
	INuiSensor* tempSensor;
	int numSensors = 0;
	if (NuiGetSensorCount(&numSensors) != S_OK ) 
	{
		std::cout << "Kinect Error: Sensor lookup failed, Probably no sensors connected" << std::endl;
		std::getchar();
		return false;
	}

	// Loop through all sensors
	for (int i = 0; i < numSensors; ++i)
	{
		// Create the sensor so we can check status, if we can't create it, move on to the next
		if (NuiCreateSensorByIndex(i, &tempSensor) < 0)
		{
			continue;
		}

		// Get the status of the sensor, and if connected, then we can initialize it
		if (tempSensor->NuiStatus() == S_OK)
		{
			kinectSensor = tempSensor;
			break;
		}

		// This sensor wasn't OK, so release it since we're not using it
		tempSensor->Release();
	}

	if (kinectSensor != NULL)
	{
		// Initialize the Kinect and specify that we'll be using skeleton
		if (kinectSensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_SKELETON) >= 0)
		{
			// Create an event that will be signaled when skeleton data is available
			nextSkeletonUpdate = CreateEventW(NULL, TRUE, FALSE, NULL);

			// Open a skeleton stream to receive skeleton data
			if (kinectSensor->NuiSkeletonTrackingEnable(nextSkeletonUpdate, 0) < 0)
			{
				std::cout << "Kinect Error: Couldn't open a skeleton stream" << std::endl;
				std::getchar();
				return false;
			}
		}
		else 
		{
			std::cout << "Kinect Error: Sensor failed initialization" << std::endl;
			std::getchar();
			return false;
		}
	}
	else
	{
		std::cout << "Kinect Error: No sensor could be initialized" << std::endl;
		std::getchar();
		return false;
	}

	return true;
}

bool KinectInput::startTracking()
{
	if (kinectSensor != NULL)
	{
		// Start tracking, also set whether the user is seated or standing.
		kinectSensor->NuiSkeletonTrackingEnable(nextSkeletonUpdate, sitMode ? NUI_SKELETON_TRACKING_FLAG_ENABLE_SEATED_SUPPORT : 0);
		return true;
	}
	else
	{
		std::cout << "Kinect Error: Sensor can't start tracking until it is initialized" << std::endl;
		std::getchar();
		return false;
	}
}

void KinectInput::update()
{
	if (kinectSensor == NULL)
	{
		return;
	}

	// Wait for 0ms, just quickly test if it is time to process a skeleton
	if (WaitForSingleObject(nextSkeletonUpdate, 0) == WAIT_OBJECT_0)
	{
		processSkeletonData();
	}
}

void KinectInput::processSkeletonData()
{
	
	NUI_SKELETON_FRAME skeletonFrame = { 0 };

	// Get the skeleton frame that is ready
	if (kinectSensor->NuiSkeletonGetNextFrame(0, &skeletonFrame) >= 0)
	{	
		// Can replace 1 with NUI_SKELETON_COUNT to track multiple people, currently out of scope for our current game.
		for (int i = 0; i < 1; i++)
		{
			NUI_SKELETON_DATA &skeleton = skeletonFrame.SkeletonData[i];
			calculateHandPos(skeleton);
		}
	}
}

void KinectInput::calculateHandPos(NUI_SKELETON_DATA skeleton)
{
	if (skeleton.eTrackingState == NUI_SKELETON_TRACKED)
	{
		handPos.x = skeleton.SkeletonPositions[selectedJoint].x;
		handPos.y = skeleton.SkeletonPositions[selectedJoint].y;
		handPos.z = skeleton.SkeletonPositions[selectedJoint].z;
	}
}

void KinectInput::switchHand(TrackingPoint newHand)
{
	switch (newHand)
	{
	case LeftHand:
		selectedJoint = NUI_SKELETON_POSITION_HAND_LEFT;
		break;
	case RightHand:
		selectedJoint = NUI_SKELETON_POSITION_HAND_RIGHT;
		break;
	default:
		std::cout << "Kinect Error: Invalid joint selected" << std::endl;
		std::getchar();
		break;
	}
}

Vec3 KinectInput::getHandPos()
{
	return handPos;
}