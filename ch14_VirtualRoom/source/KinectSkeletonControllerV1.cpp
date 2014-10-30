//#############################################################################
//  File:      KinectSkeletonControllerV1.cpp
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include "KinectSkeletonControllerV1.h"

using namespace VirtualRoom;

NUI_SKELETON_DATA				_skeleton;												//!< skeleton data
NUI_SKELETON_BONE_ORIENTATION	_orientation[NUI_SKELETON_POSITION_COUNT];				//!< skeletons bone orientations
NUI_TRANSFORM_SMOOTH_PARAMETERS _smoothingParameters = {0.5f, 0.1f, 0.7f, 0.1f, 0.1f};	//!< skeleton filter parameters
SLfloat							_cameraHeight;											//!< camera height
//-----------------------------------------------------------------------------
KinectControllerV1::KinectControllerV1()
{
	this->_started = false;
	this->seatedMode = false;
	int activeDeviceNumber = 0;
	this->initDeviceAndStreams(activeDeviceNumber);
}
//-----------------------------------------------------------------------------
KinectControllerV1::~KinectControllerV1()
{
	if(this->_started)
	{
		this->sensor->NuiShutdown();
	}
}
//-----------------------------------------------------------------------------
/*! 
checks the number of connected kinect devices
*/
bool KinectControllerV1::kinectNumberOfConnectedDevices(int* numberOfDevices)
{
	if (NuiGetSensorCount(numberOfDevices) < 0)
		return false;

	return true;
}
//-----------------------------------------------------------------------------
/*! 
initalize the kinect device
*/
bool KinectControllerV1::initDeviceAndStreams(int& deviceNr)
{
	// Get a working kinect sensor
	int numSensors;

	if (!kinectNumberOfConnectedDevices(&numSensors) || numSensors < deviceNr)
		return false;

	if (NuiCreateSensorByIndex(deviceNr, &this->sensor) < 0)
		return false;

	this->_started = true;

	// Initialize sensor
	this->sensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_SKELETON);

	this->m_hNextSkeletonEvent = CreateEvent( NULL, TRUE, FALSE, NULL );

	HRESULT hr;
	if (this->seatedMode)
	{
		hr = this->sensor->NuiSkeletonTrackingEnable(m_hNextSkeletonEvent, NUI_SKELETON_TRACKING_FLAG_TITLE_SETS_TRACKED_SKELETONS | NUI_SKELETON_TRACKING_FLAG_ENABLE_SEATED_SUPPORT);
	}
	else
	{
		hr = this->sensor->NuiSkeletonTrackingEnable(m_hNextSkeletonEvent, NUI_SKELETON_TRACKING_FLAG_TITLE_SETS_TRACKED_SKELETONS);
	}

	if(FAILED(hr))
		return false;

	return true;
}
//-----------------------------------------------------------------------------
/*! 
get current skeleton data from the kinect
*/
void KinectControllerV1::refreshKinectData(SkeletonDataRaw* data, SkeletonData* default)
{
	HRESULT hr;
	NUI_SKELETON_FRAME *skeletonFrame = new NUI_SKELETON_FRAME;

	// Wait for 0ms, just quickly test if it is time to process a skeleton
	if ( WAIT_OBJECT_0 == WaitForSingleObject(m_hNextSkeletonEvent, 0) )
	{
		// Get the skeleton frame that is ready
		if (FAILED(this->sensor->NuiSkeletonGetNextFrame(0, skeletonFrame)))
		{
			delete skeletonFrame;
		}

		// Smooth the skeleton joint positions
		this->sensor->NuiTransformSmooth(skeletonFrame, &_smoothingParameters);

		// Check if the skeleton to track has to be set newly
		bool skeletonFound = false;

		// get information about the first skeleton
		for (int i = NUI_SKELETON_COUNT-1; i >= 0; i--)
		{
			_skeleton = skeletonFrame->SkeletonData[i];

			if (_skeleton.eTrackingState == NUI_SKELETON_TRACKED)
			{				
				_cameraHeight = skeletonFrame->vFloorClipPlane.w;

				NuiSkeletonCalculateBoneOrientations(&_skeleton, _orientation);
				skeletonFound = true;
				break;
			}
		}

		// Set new skeleton to load
		if (!skeletonFound)
		{
			setClosestSkeletonForTraking(skeletonFrame);
		}
	}

	delete skeletonFrame;

	// fill data
	NUI_SKELETON_DATA* skeleton;
	NUI_SKELETON_BONE_ORIENTATION* orientation;

	skeleton	= getSkeleton();
	orientation = getOrientation();

	// set position
	data->position.x = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_HIP_CENTER].x;
	data->position.y = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_HIP_CENTER].y;
	data->position.z = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_HIP_CENTER].z;

	// set bones
	VR_SKELETON_POSITION_INDEX tmpIndex;
	VR_SKELETON_POSITION_INDEX parentIndex;

	for(SLint i = 0; i < NUI_SKELETON_POSITION_COUNT; i++)
	{
		// ------------- hack for kinectV2 hierarchy compability ------------------------------
		tmpIndex = (VR_SKELETON_POSITION_INDEX) i;
		if(i == VR_SKELETON_POSITION_NECK)
			tmpIndex = VR_SKELETON_POSITION_SPINE_SHOULDER;

		parentIndex = default->bones[tmpIndex].parent;
		if(i == VR_SKELETON_POSITION_SHOULDER_LEFT || i == VR_SKELETON_POSITION_SHOULDER_RIGHT)
			parentIndex = VR_SKELETON_POSITION_NECK;
		// ------------- end hack -------------------------------------------------------------

		// set rotation
		data->bones[tmpIndex].rotation	= SLQuat4f(	orientation[i].hierarchicalRotation.rotationQuaternion.x,
			orientation[i].hierarchicalRotation.rotationQuaternion.y,
			orientation[i].hierarchicalRotation.rotationQuaternion.z,
			orientation[i].hierarchicalRotation.rotationQuaternion.w
			);
		data->bones[tmpIndex].absoluteRotation = SLQuat4f(	orientation[i].absoluteRotation.rotationQuaternion.x,
			orientation[i].absoluteRotation.rotationQuaternion.y,
			orientation[i].absoluteRotation.rotationQuaternion.z,
			orientation[i].absoluteRotation.rotationQuaternion.w
			);

		// calculate bone length
		SLfloat transX = skeleton->SkeletonPositions[i].x-skeleton->SkeletonPositions[parentIndex].x;
		SLfloat transY = skeleton->SkeletonPositions[i].y-skeleton->SkeletonPositions[parentIndex].y;
		SLfloat transZ = skeleton->SkeletonPositions[i].z-skeleton->SkeletonPositions[parentIndex].z;
		data->bones[tmpIndex].length = sqrt(transX*transX+transY*transY+transZ*transZ);

		// set state
		data->bones[tmpIndex].state = (VR_TRACKING_STATE)skeleton->eSkeletonPositionTrackingState[i];		
	}
}
//-----------------------------------------------------------------------------
/*! 
get the closest skeleton for the tracking
*/
void KinectControllerV1::setClosestSkeletonForTraking(NUI_SKELETON_FRAME *skeletonFrame)
{
	// Start with a far distance
	float closestDistance = 10000.0f;
	DWORD closestIDs[NUI_SKELETON_MAX_TRACKED_COUNT] = {0, 0};

	for (int i = 0; i < NUI_SKELETON_COUNT; i++)
	{
		const NUI_SKELETON_DATA & skeleton = skeletonFrame->SkeletonData[i];
		if (skeleton.eTrackingState != NUI_SKELETON_NOT_TRACKED)
		{
			if (skeleton.Position.z < closestDistance)
			{
				closestIDs[0] = skeleton.dwTrackingID;
				closestDistance = skeleton.Position.z;
			}
		}
	}

	if (closestIDs[0] > 0)
	{
		this->sensor->NuiSkeletonSetTrackedSkeletons(closestIDs);
	}
}
//-----------------------------------------------------------------------------
/*! 
returns the camera height
*/
SLfloat KinectControllerV1::getCameraHeight()
{
	return _cameraHeight;
}
//-----------------------------------------------------------------------------
/*! 
returns the current bone positions
*/
NUI_SKELETON_DATA* KinectControllerV1::getSkeleton()
{
	return &_skeleton;
}
//-----------------------------------------------------------------------------
/*! 
returns the current bone orientations
*/
NUI_SKELETON_BONE_ORIENTATION* KinectControllerV1::getOrientation()
{
	return _orientation;
}
//-----------------------------------------------------------------------------
/*! 
shut down the kinect device
*/
void KinectControllerV1::shutdown()
{
	if (this->_started = true)
	{
		this->sensor->NuiShutdown();
		this->_started = false;
	}
}
//-----------------------------------------------------------------------------
/*! 
change tracking mode
*/
bool KinectControllerV1::changeMode()
{
	HRESULT hr;
	if(!this->seatedMode)
	{
		hr = this->sensor->NuiSkeletonTrackingEnable(m_hNextSkeletonEvent, NUI_SKELETON_TRACKING_FLAG_ENABLE_SEATED_SUPPORT);
	}
	else
	{
		hr = this->sensor->NuiSkeletonTrackingEnable(m_hNextSkeletonEvent, 0);
	}

	if(FAILED(hr))
		return false;

	this->seatedMode = !this->seatedMode;

	return true;
}
//-----------------------------------------------------------------------------