//#############################################################################
//  File:      KinectSkeletonControllerV1.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VRKINECTCONTROLLERV1_H
#define VRKINECTCONTROLLERV1_H

#include <stdafx.h>           // precompiled headers

#include "KinectSkeletonController.h"

#include <NuiApi.h>
#include <NuiImageCamera.h>
#include <NuiSensor.h>

namespace VirtualRoom
{
	//-----------------------------------------------------------------------------
	//! controller for kinect one
	/*!      
	interface to the kinect sdk
	*/
	//-----------------------------------------------------------------------------
	class KinectControllerV1 : public KinectController
	{
	public:
		KinectControllerV1();
		virtual ~KinectControllerV1();

		virtual void refreshKinectData							(SkeletonDataRaw* data, SkeletonData* default);
		virtual bool initDeviceAndStreams						(int& deviceNr);
		virtual void shutdown									();
		virtual bool changeMode									();
		virtual NUI_SKELETON_DATA* getSkeleton					();
		virtual NUI_SKELETON_BONE_ORIENTATION* getOrientation	();
		virtual SLfloat getCameraHeight							();
		bool kinectNumberOfConnectedDevices						(int* numberOfDevices);
		
	private:
		// Kinect variables
		HANDLE		m_hNextSkeletonEvent;	//!< skeleton event
		INuiSensor* sensor;					//!< kinect sensor
		bool		seatedMode;				//!< is seated mode active

		void setClosestSkeletonForTraking(NUI_SKELETON_FRAME *skeletonFrame);
	};
}
//-----------------------------------------------------------------------------
#endif