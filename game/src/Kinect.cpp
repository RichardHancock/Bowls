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
		cgg::loge("Kinect Error: Sensor lookup failed, Probably no sensors connected");
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
				cgg::loge("Kinect Error: Couldn't open a skeleton stream");
				return false;
			}
		}
		else 
		{
			cgg::loge("Kinect Error: Sensor failed initialization");
			return false;
		}
	}
	else
	{
		cgg::loge("Kinect Error: No sensor could be initialized");
		return false;
	}

	return true;
}

bool KinectInput::startTracking()
{
	if (kinectSensor != NULL)
	{
		HRESULT hr;
		// Start tracking, also set whether the user is seated or standing.
		hr = kinectSensor->NuiSkeletonTrackingEnable(nextSkeletonUpdate, sitMode ? NUI_SKELETON_TRACKING_FLAG_ENABLE_SEATED_SUPPORT : 0);
		switch (hr)
		{
		case ERROR_INVALID_OPERATION:
			cgg::loge("ERROR_INVALID_OPERATION");
			break;
		case E_INVALIDARG:
			cgg::loge("E_INVALIDARG");
			break;
		case E_OUTOFMEMORY:
			cgg::loge("E_OUTOFMEMORY");
			break;
		default:
			break;
		}
		return true;
	}
	else
	{
		cgg::loge("Kinect Error: Sensor can't start tracking until it is initialized");
		return false;
	}
}

void KinectInput::update()
{
	if (kinectSensor == NULL) { return; }

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
		///Skeleton Data Smoothing Algorithm parameters
		const NUI_TRANSFORM_SMOOTH_PARAMETERS smoothParams = { 0.5f, 0.5f, 0.5f, 0.05f, 0.04f };
		NuiTransformSmooth(&skeletonFrame, &smoothParams);

		// Can replace 1 with NUI_SKELETON_COUNT to track multiple people, currently out of scope for our current game.
		for (int i = 0; i < NUI_SKELETON_COUNT; i++)
		{
			NUI_SKELETON_TRACKING_STATE trackingState =  skeletonFrame.SkeletonData[i].eTrackingState;
			if (trackingState == NUI_SKELETON_TRACKED)
			{
				NUI_SKELETON_DATA &skeleton = skeletonFrame.SkeletonData[i];
				calculateHandPos(skeleton);
			}
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
		kinectPosConversion();
	}
}

void KinectInput::kinectPosConversion()
{
	//Scale it up a bit to make movements noticeable
	handPos *= 5;

	cgg::Vec3 flippedKinectPos = handPos;

	flippedKinectPos.x = -handPos.z;
	flippedKinectPos.y = handPos.y;
	flippedKinectPos.z = handPos.x;

	handPos = flippedKinectPos;
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
		cgg::logw("Kinect Warning: Invalid joint selected, old joint preserved");
		break;
	}
}

cgg::Vec3 KinectInput::getHandPos()
{
	return handPos;
}