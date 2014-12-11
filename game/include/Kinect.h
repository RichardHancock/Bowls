#pragma once
#include <Windows.h>
#include <NuiApi.h>
#include "cgg/cgg.h"

enum TrackingPoint
{
	LeftHand,
	RightHand
};

class KinectInput
{
private:
	INuiSensor* kinectSensor;

	HANDLE nextSkeletonUpdate;

	bool sitMode;
	NUI_SKELETON_POSITION_INDEX selectedJoint;
	cgg::Vec3 handPos;

	bool initSensor();
	void processSkeletonData();
	void calculateHandPos(NUI_SKELETON_DATA);
public:
	KinectInput(bool, TrackingPoint);
	~KinectInput();

	void switchHand(TrackingPoint);
	bool startTracking();
	void update();
	cgg::Vec3 getHandPos();
};
