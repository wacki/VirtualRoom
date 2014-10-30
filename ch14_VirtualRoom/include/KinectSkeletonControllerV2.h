//#############################################################################
//  File:      KinectSkeletonControllerV2.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VRKINECTCONTROLLERV2_H
#define VRKINECTCONTROLLERV2_H

#ifdef ISWIN8
#include <Kinect.h>
#endif

#include "KinectSkeletonController.h"

namespace VirtualRoom
{

	//-----------------------------------------------------------------------------
	//! controller for kinect two
	/*!      
	interface to the kinect sdk
	*/
	//-----------------------------------------------------------------------------
	class KinectControllerV2 : public KinectController
	{
#ifdef ISWIN8
	public:
		KinectControllerV2();
		virtual ~KinectControllerV2();

		virtual void				refreshKinectData(SkeletonDataRaw* data, SkeletonData* default);
		virtual Joint*				getSkeleton();
		virtual JointOrientation*	getOrientation();
		virtual float				getCameraHeight();

	private:
		IKinectSensor*		_sensor;						//!< kinect device sensor
		ICoordinateMapper*  _coordinateMapper;				//!< kinect coordinate mapper
		IBodyFrameReader*	_bodyFrameReader;				//!< skeleton body reader

		Joint				_skeleton[JointType_Count];		//!< skeleton positions
		JointOrientation	_orientation[JointType_Count];	//!< skeleton orientations
		float				_cameraHeight;					//!< camera height
		UINT64				_actTrackingId;					//!< skeleton tracking id

		virtual void		initDeviceAndStreams();

		// Safe release for interfaces
		template<class Interface>
		inline void SafeRelease(Interface *& pInterfaceToRelease)
		{
			if (pInterfaceToRelease != NULL)
			{
				pInterfaceToRelease->Release();
				pInterfaceToRelease = NULL;
			}
		}
#endif
	};
}
//-----------------------------------------------------------------------------
#endif