#pragma once
#include <Windows.h>
#include <NuiApi.h>
#include "Vec3.h"

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
	Vec3 handPos;

	bool initSensor();
	void processSkeletonData();
	void calculateHandPos(NUI_SKELETON_DATA);
public:
	KinectInput(bool, TrackingPoint);
	~KinectInput();

	void switchHand(TrackingPoint);
	bool startTracking();
	void update();
	Vec3 getHandPos();
};