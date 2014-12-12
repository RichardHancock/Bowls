#pragma once
#include <Windows.h>
#include <NuiApi.h>
#include "cgg/cgg.h"

/// Kinect Tracking Points
enum TrackingPoint
{
	LeftHand, ///< Left Hand
	RightHand ///< Right Hand
};

/**
@brief Kinect Input Manager

Manages all of the Kinect's initialization, updating and output.
*/
class KinectInput
{
private:

	///Pointer to a Kinect Sensor
	INuiSensor* kinectSensor;
	///Handle that indicates when a skeleton is ready to process
	HANDLE nextSkeletonUpdate;
	///Whether the Kinect should look for standing or sitting skeletons.
	bool sitMode;
	///Which joint we want the Kinect to track
	NUI_SKELETON_POSITION_INDEX selectedJoint;
	///Current position of the selected joint
	cgg::Vec3 handPos;

	/**
	@brief Initializes the Kinect Sensor
	@return bool - Was Kinect initialized
	*/
	bool initSensor();

	/**
	@brief Processes a Skeleton

	Fetches the Skeleton and passes it through to calculateHandPos
	*/
	void processSkeletonData();

	/**
	@brief Calculates the selected hands position
	
	@param NUI_SKELETON_DATA - The skeleton data
	*/
	void calculateHandPos(NUI_SKELETON_DATA);
public:
	/**
	@brief Creates the Kinect Input Manager
	
	@param bool - Enable sitMode for the Kinect
	@param TrackingPoint - Which joint to track
	*/
	KinectInput(bool, TrackingPoint);

	~KinectInput();

	/**
	@brief Switch which hand the Kinect tracks
	
	@param TrackingPoint - Which joint to track
	*/
	void switchHand(TrackingPoint);

	/**
	@brief Start tracking skeletons 
	
	Must be run to get any skeleton data processed
	@return bool - Was tracking started successfully
	*/
	bool startTracking();

	/**
	@brief Update the Kinect Input Managers Internals

	Checks if there is a skeleton available, if so it calls all the
	necessary functions to process the skeleton
	*/
	void update();

	/**
	@brief Get the current tracked hand position
	
	Gives positions in meters. X: -2.2 to 2.2, Y: -1.6 to 1.6,
	Z: 0.0 to 4.0 (These are Approximate)
	@return cgg::Vec3 - The hand position
	*/
	cgg::Vec3 getHandPos();
};
