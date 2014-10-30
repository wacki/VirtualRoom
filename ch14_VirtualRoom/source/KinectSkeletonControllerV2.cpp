//#############################################################################
//  File:      KinectSkeletonControllerV2.cpp
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include "KinectSkeletonControllerV2.h"
#include <d2d1.h>

using namespace VirtualRoom;

#ifdef ISWIN8
//-----------------------------------------------------------------------------
KinectControllerV2::KinectControllerV2() : _actTrackingId(-1)
{
	initDeviceAndStreams();
}
//-----------------------------------------------------------------------------
KinectControllerV2::~KinectControllerV2()
{
	//SafeRelease(m_pD2DFactory);
	SafeRelease(_bodyFrameReader);
	SafeRelease(_coordinateMapper);

	// close the Kinect Sensor
	if (_sensor)
	{
		_sensor->Close();
	}
	SafeRelease(_sensor);
}
//-----------------------------------------------------------------------------
/*!
initalize the kinect device
*/
void KinectControllerV2::initDeviceAndStreams()
{
	// init
	HRESULT hr;
	_started = true;

	hr = GetDefaultKinectSensor(&_sensor);
	if (FAILED(hr))
	{
		_started = false;
	}

	if (_sensor && _started)
	{
		// Initialize the Kinect and get coordinate mapper and the body reader
		IBodyFrameSource* pBodyFrameSource = NULL;

		hr = _sensor->Open();

		if (SUCCEEDED(hr))
		{
			hr = _sensor->get_CoordinateMapper(&_coordinateMapper);
		}

		if (SUCCEEDED(hr))
		{
			hr = _sensor->get_BodyFrameSource(&pBodyFrameSource);
		}

		if (SUCCEEDED(hr))
		{
			hr = pBodyFrameSource->OpenReader(&_bodyFrameReader);
		}

		SafeRelease(pBodyFrameSource);
	}

	BOOLEAN av;
	hr = _sensor->get_IsAvailable(&av);

	if (!_sensor || FAILED(hr) || !av)
	{
		_started = false;
	}
}
//-----------------------------------------------------------------------------
/*!
get current skeleton data from the kinect
*/
void KinectControllerV2::refreshKinectData(SkeletonDataRaw* data, SkeletonData* default)
{
	if (!_bodyFrameReader || !_started)
	{
		return;
	}

	IBodyFrame* pBodyFrame = NULL;

	HRESULT hr = _bodyFrameReader->AcquireLatestFrame(&pBodyFrame);

	if (SUCCEEDED(hr))
	{
		INT64 nTime = 0;

		hr = pBodyFrame->get_RelativeTime(&nTime);

		IBody* ppBodies[BODY_COUNT] = {0};

		if (SUCCEEDED(hr))
		{
			hr = pBodyFrame->GetAndRefreshBodyData(_countof(ppBodies), ppBodies);
		}

		if (SUCCEEDED(hr))
		{
			Vector4 floorClipPlane;
			pBodyFrame->get_FloorClipPlane(&floorClipPlane);
			_cameraHeight = floorClipPlane.w;

			// reset bone states
			for(int i = 0; i < JointType_Count; i++)
				_skeleton[i].TrackingState = TrackingState_NotTracked;

			// Check if the skeleton to track has to be set newly
			bool trackedBodyFound = false;
			float closestDistance = 10000.0f;
			bool foundCloserTrackingID = false;
			UINT64 closestTrackingID = 0;

			for (int i = 0; i < BODY_COUNT; ++i)
			{
				IBody* pBody = ppBodies[i];
				if (pBody)
				{
					BOOLEAN bTracked = false;
					hr = pBody->get_IsTracked(&bTracked);

					UINT64 tackingID = 0;
					HRESULT hr2 = pBody->get_TrackingId(&tackingID);

					if (SUCCEEDED(hr) && bTracked && SUCCEEDED(hr2))
					{
						if (tackingID == this->_actTrackingId)
						{
							// refresh skeleton data
							hr = pBody->GetJoints(_countof(_skeleton), _skeleton);
							pBody->GetJointOrientations(_countof(_orientation), _orientation);
							trackedBodyFound = true;
							break;
						}
						else
						{
							// Get position for eventualy setting the body for tracking the next time
							hr = pBody->GetJoints(_countof(_skeleton), _skeleton);
							if (_skeleton[JointType_SpineBase].Position.Z < closestDistance)
							{
								closestDistance = _skeleton[JointType_SpineBase].Position.Z;
								foundCloserTrackingID = true;
								closestTrackingID = tackingID;
							}
						}
					}
				}
			}

			// Set new the nearest body to be checked at next frame
			if (!trackedBodyFound && foundCloserTrackingID)
			{
				this->_actTrackingId = closestTrackingID;
			}
		}

		for (int i = 0; i < _countof(ppBodies); ++i)
		{
			SafeRelease(ppBodies[i]);
		}
	}

	SafeRelease(pBodyFrame);

	// fill data
	Joint*				skeleton;
	JointOrientation*	orientation;

	skeleton	= getSkeleton();
	orientation = getOrientation();

	// set position
	data->position.x = skeleton[JointType_SpineBase].Position.X;
	data->position.y = skeleton[JointType_SpineBase].Position.Y;
	data->position.z = skeleton[JointType_SpineBase].Position.Z;

	// set bones
	for(SLint i = 0; i < VR_SKELETON_POSITION_COUNT; i++)
	{
		// set rotation
		SLQuat4f cRot = SLQuat4f(	orientation[i].Orientation.x,
			orientation[i].Orientation.y,
			orientation[i].Orientation.z,
			orientation[i].Orientation.w
			);
		SLQuat4f pRot = SLQuat4f(	orientation[(JointType)default->bones[i].parent].Orientation.x,
			orientation[(JointType)default->bones[i].parent].Orientation.y,
			orientation[(JointType)default->bones[i].parent].Orientation.z,
			orientation[(JointType)default->bones[i].parent].Orientation.w
			);

		if(i != 0)
		{
			data->bones[i].rotation = pRot.conjugated() * cRot;
		}else{
			data->bones[i].rotation = cRot;
		}
		data->bones[i].absoluteRotation = cRot;

		// calculate bone length
		SLfloat transX = skeleton[(JointType)i].Position.X-skeleton[(JointType)default->bones[i].parent].Position.X;
		SLfloat transY = skeleton[(JointType)i].Position.Y-skeleton[(JointType)default->bones[i].parent].Position.Y;
		SLfloat transZ = skeleton[(JointType)i].Position.Z-skeleton[(JointType)default->bones[i].parent].Position.Z;
		data->bones[i].length = sqrt(transX*transX+transY*transY+transZ*transZ);

		// set state
		data->bones[i].state = (VR_TRACKING_STATE)skeleton[i].TrackingState;
	}
}
//-----------------------------------------------------------------------------
/*!
returns the current bone positions
*/
Joint* KinectControllerV2::getSkeleton()
{
	return _skeleton;
}
//-----------------------------------------------------------------------------
/*!
returns the current bone orientations
*/
JointOrientation* KinectControllerV2::getOrientation()
{
	return _orientation;
}
//-----------------------------------------------------------------------------
/*!
returns the camera height
*/
float KinectControllerV2::getCameraHeight()
{
	return _cameraHeight;
}
//-----------------------------------------------------------------------------
#endif