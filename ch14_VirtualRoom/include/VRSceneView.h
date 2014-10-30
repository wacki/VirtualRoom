//#############################################################################
//  File:      VRSceneView.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VRSCENEVIEW_H
#define VRSCENEVIEW_H

#include <SLSceneView.h>

#include "SLPerson.h"
#include "VRNetworkClient.h"
#include "VRGameScene.h"
#include "VRColorSphereGameScene.h"
#include "VRTicTacToeGameScene.h"
#include "VRConnectFourGameScene.h"
#include "VRTurnTableGameScene.h"
#include "KinectManager.h"

#include "VRSkeletonKinectBinding.h"
#include "SLSkeleton.h"


#define SERVER_ADDR_ARGUMENT_INDEX 1
#define CLIENT_TYPE_ARGUMENT_INDEX 0
#define SERVER_PORT 60000
#define NETWORK_DATA_SAMPLE_COUNT 1000
#define CAMERA_LOOP_TIMESPAN_SEC 100 // TODO: change back to lower number

//-----------------------------------------------------------------------------
//! network data storage
/*!
stores the collected data of the network test
*/
struct NetworkDataStorage
{
	bool	collectNetworkData;
	double	collectingStartTime;
	SLint	currentDataCollectingIndex;
	double	lastDataPacketReceived[NETWORK_DATA_SAMPLE_COUNT];
	SLint	numberOfReceivedDataPackets[NETWORK_DATA_SAMPLE_COUNT];
	double	pingData[NETWORK_DATA_SAMPLE_COUNT];
};

//-----------------------------------------------------------------------------
//! virtual room scene view
/*!      
extended SLSceneView for the virtual room project
*/
//-----------------------------------------------------------------------------
class VRSceneView : public SLSceneView
{
public:
   VRSceneView() : _isObserverCamLoopActive(false) { name("vrsceneview"); _networkDataStorage.collectNetworkData = false; }
   ~VRSceneView();
   
   void							onStartup(vector<string>& onStartup);
   void							postSceneLoad();
   void							preDraw();
   KinectManager*				kinect() {return _kinect;}
   virtual SLbool				onKeyPress(const SLKey key, const SLKey mod);

protected:
	KinectManager*				_kinect;			//!< kinect data manager
	VRPerson					_vrPerson;			//!< skeleton data manager
	VirtualRoom::NetworkClient*	_networkClient;		//!< network connection manager
	SLMat4f*					_initMatrixPersons;	//!< initial person matrizes
	VR_MODEL*					_initModels;		//!< initial models for the persons
	GameScene*					_gameScene;			//!< game scene
	Network::ConnectingAnswer*	_connectingAnswer;	//!< answer of the connection request
	SLint						_numberOfPlayerCams;//!< number of player cameras for the loop
	string						_serverAddress;		//!< server address
	VR_CLIENT_TYPE				_clientType;		//!< client type
	SLCamera*					_freeCam;			//!< game camera
	SLCamera*					_freeCam2;			//!< game camera 2 (with animation)
	VRSkeletonKinectBinding		_tempTestBinding;	//!< kinect to skeleton binding
	SLBoneNode					*_tempBoneRoot;		//!< skeleton root bone

    SLNode*                      _tempTestNode;

	void						initCommandLineArguments(vector<string>& commandLineArguments);

private:
	NetworkDataStorage			_networkDataStorage;		//!< network data storage
	SLbool						_isObserverCamLoopActive;	//!< loop over all the cameras on observer active
	SLuint						_currentActiveCamIndex;		//!< current active camera index for loop
	double						_lastCamChangeTime;			//!< last change of the camera
	SLTimer						_camLoopTimer;				//!< timer for camera loop

	void collectNetworkData();
};
//-----------------------------------------------------------------------------
#endif