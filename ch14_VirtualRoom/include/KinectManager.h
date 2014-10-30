//#############################################################################
//  File:      KinectManager.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VRKINECTMANAGER_H
#define VRKINECTMANAGER_H

#include "VRTypes.h"
#include "KinectSkeletonControllerV2.h"
#include "KinectSkeletonControllerV1.h"

namespace VirtualRoom
{

	#define Z_OFFSET_METERS 2.4f

	//-----------------------------------------------------------------------------
	//! kinect data manager
	/*!      
	kinect data manager class for dynamic use of kinect version one and two
	*/
	//-----------------------------------------------------------------------------
	class KinectManager
	{
	public:
		KinectManager();
		virtual ~KinectManager();

		virtual void			refreshKinectData	();
		SkeletonDataRaw*		getKinectData		();
		void					setDefaultData		(SkeletonData* default){_default = default;}

	private:
		KinectController*		_kc;		//!< kinect controller
			
		SkeletonDataRaw*		_data;		//!< skeleton raw data
		SkeletonData*			_default;	//!< skeleton processed data
	};

}
//-----------------------------------------------------------------------------
#endif