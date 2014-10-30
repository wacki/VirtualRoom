//#############################################################################
//  File:      KinectSkeletonController.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################


#ifndef VRKINECTCONTROLLER_H
#define VRKINECTCONTROLLER_H

#include "VRTypes.h"

namespace VirtualRoom
{
	//-----------------------------------------------------------------------------
	//! kinect controller base class
	/*!      
	defines all needed functions for the individual kinect controllers
	*/
	//-----------------------------------------------------------------------------
	class KinectController
	{
	public:
		KinectController(){}

		virtual	void	refreshKinectData	(SkeletonDataRaw* data, SkeletonData* default) = 0;
		inline	bool	isStarted			() const {return _started;}

	protected:
		bool	_started;	//!< is the kinnect started
	};
}
//-----------------------------------------------------------------------------
#endif