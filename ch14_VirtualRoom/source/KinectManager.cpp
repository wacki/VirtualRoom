//#############################################################################
//  File:      KinectManager.cpp
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include "KinectManager.h"

using namespace VirtualRoom;

//-----------------------------------------------------------------------------
KinectManager::KinectManager()
	: _kc(NULL)
{
	_data = new SkeletonDataRaw;
	// init length with 0.0f
	for(int i = 0; i < VR_SKELETON_POSITION_COUNT; i++)
		_data->bones[i].length = 0.0f;

	// kinect v2 works only for win8
#ifdef ISWIN8
	_kc = new KinectControllerV2();

	if(!_kc || !_kc->isStarted()){
		delete _kc;
		_kc = new KinectControllerV1();
	}
#else
	_kc = new KinectControllerV1();
#endif

}
//-----------------------------------------------------------------------------
KinectManager::~KinectManager()
{
	delete _data;

	if(_kc)
		delete _kc;
}
//-----------------------------------------------------------------------------
/*!
refresh kinect skeleton data
*/
void KinectManager::refreshKinectData()
{
	if(_kc && _kc->isStarted())
		_kc->refreshKinectData(_data, _default);

	// normalize skeleton position
	_data->position.z -= Z_OFFSET_METERS;	
}
//-----------------------------------------------------------------------------
/*!
get the current data from the kinect
*/
SkeletonDataRaw* KinectManager::getKinectData()
{
	return _data;
}
//-----------------------------------------------------------------------------


