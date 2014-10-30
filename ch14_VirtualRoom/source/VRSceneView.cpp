//#############################################################################
//  File:      VRSceneView.cpp
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include "VRSceneView.h"
#include "SLAnimation.h"
#include "SLTrackingCamera.h"

//-----------------------------------------------------------------------------
VRSceneView::~VRSceneView()
{
	delete		_networkClient;
	if(_initMatrixPersons)
		delete[] _initMatrixPersons;
	if(_initModels)
		delete[] _initModels;
	if(_gameScene)
		delete	_gameScene;
	if(_connectingAnswer)
		delete _connectingAnswer;

	if (_kinect)
		delete _kinect;

}
//-----------------------------------------------------------------------------
/*! 
handles command line arguments
*/
void VRSceneView::initCommandLineArguments(vector<string>& commandLineArguments)
{
	// set server address
	if(commandLineArguments.size() > SERVER_ADDR_ARGUMENT_INDEX)
		_serverAddress = commandLineArguments.at(SERVER_ADDR_ARGUMENT_INDEX);
	else
		_serverAddress = "localhost";

	// set client type
	if(commandLineArguments.size() > CLIENT_TYPE_ARGUMENT_INDEX)
		_clientType = (VR_CLIENT_TYPE)atoi(commandLineArguments.at(CLIENT_TYPE_ARGUMENT_INDEX).c_str());
	else
		_clientType = VR_CLIENT_TYPE_PLAYER;
}
//-----------------------------------------------------------------------------
/*! 
starts the connection to the server
*/
void VRSceneView::onStartup(vector<string>& cmdLineArgs)
{
	initCommandLineArguments(cmdLineArgs);
	_networkClient = new NetworkClient(_serverAddress, _clientType);

	if(_clientType == VR_CLIENT_TYPE_PLAYER)
	{
		_kinect = new KinectManager;
		_kinect->setDefaultData(_vrPerson.getDefaultData());
	}
	else
	{
		_kinect = NULL;
	}

	bool noNetworkConnection = false;
	_connectingAnswer = NULL;
	_initMatrixPersons = NULL;
	_initModels = NULL;
	
	_networkClient->connect();
	_networkClient->waitForConnect();
	if(_networkClient->isConnected())
	{
		_networkClient->sendConnectingQuestion();
		_networkClient->waitForConnectingAnswer(&_connectingAnswer);
		if(!_networkClient->isConnected())
		{
			SL_LOG("Init positions could not be reveiced correctly\n");
			noNetworkConnection = true;
		}else
		{
			_initMatrixPersons = _connectingAnswer->_initMat;
			_initModels = _connectingAnswer->_initModels;
		}
	}
	else
	{
		SL_LOG("!!! NOT CONNECTED !!!\n");
		noNetworkConnection = true;
	}
	
	// Set Default values
	if (noNetworkConnection && _clientType == VR_CLIENT_TYPE_PLAYER)
	{
		_initMatrixPersons = new SLMat4f[1];
		_initMatrixPersons[0].identity();
		_initMatrixPersons[0].translate(0.0f, 0.0f, 1.0f);
		_initModels = new VR_MODEL[1];
		_initModels[0] = VR_HL2_CIV_MALE;
	}
}
//-----------------------------------------------------------------------------
/*! 
loads the game
*/
void VRSceneView::postSceneLoad()
{
	SLScene* s = SLScene::current;
	_gameScene = NULL;

	// create free camera
	_freeCam = new SLCamera();
	_freeCam->name("FreeCam1");
    _freeCam->position(0, 1.5f, 4.5f);
    _freeCam->lookAt(0, 0, 0);
	_freeCam->focalDist(20);
	//_freeCam->hide(); // TODO: reimplement this
	s->root3D()->addChild(_freeCam);
	camera(_freeCam);
	_numberOfPlayerCams = 0;

	// create free camera 2 (with animation)
	_freeCam2 = new SLTrackingCamera();
	_freeCam2->translate(0.0f, 4.0f, 0.0f);
	_freeCam2->focalDist(20);
	//_freeCam2->hide(); // TODO: reimplement

	SLNode* freeCam2Node = new SLNode;
	freeCam2Node->addChild(_freeCam2);
	freeCam2Node->animation(new SLAnimation(20, 5, ZAxis, 5, XAxis, loop));
	s->root3D()->addChild(freeCam2Node);

	if(_networkClient->isConnected())
	{
		_numberOfPlayerCams = _connectingAnswer->_maxPlayers;
		_vrPerson.setSkeletonModel(_initModels[_connectingAnswer->_playerIndex]);

		switch (_connectingAnswer->_gameType)
		{
		case VR_GAME_TYPE_COLOR_SPHERE:
			_gameScene = new ColorSphereGameScene(_vrPerson.getDefaultData(), _connectingAnswer->_playerIndex, _connectingAnswer->_maxPlayers, _initMatrixPersons, _initModels);
			break;
		case VR_GAME_TYPE_TURN_TABLE:
			_gameScene = new TurnTableGameScene(_vrPerson.getDefaultData(), _connectingAnswer->_playerIndex, _connectingAnswer->_maxPlayers, _initMatrixPersons, _initModels);
			break;
		case VR_GAME_TYPE_TIC_TAC_TOE:
			_gameScene = new TicTacToeGameScene(_vrPerson.getDefaultData(), _connectingAnswer->_playerIndex, _connectingAnswer->_maxPlayers, _initMatrixPersons, _initModels);
			break;
		case VR_GAME_TYPE_CONNECT_FOUR:
			_gameScene = new ConnectFourGameScene(_vrPerson.getDefaultData(), _connectingAnswer->_playerIndex, _connectingAnswer->_maxPlayers, _initMatrixPersons, _initModels);
			break;
		default:
			_gameScene = new GameScene( _vrPerson.getDefaultData(), _connectingAnswer->_playerIndex, _connectingAnswer->_maxPlayers, _initMatrixPersons, _initModels);
			break;
		}
		// Add root node
		s->root3D()->addChild(_gameScene->getRootNode());
	}else if(_clientType == VR_CLIENT_TYPE_PLAYER)
	{
		_numberOfPlayerCams = 1;
		_gameScene = new GameScene(_vrPerson.getDefaultData(), 0, 1, _initMatrixPersons, _initModels);
		// Add root node
		s->root3D()->addChild(_gameScene->getRootNode());
	}
    
    SLBox* box = new SLBox();
    _tempTestNode = new SLNode;
    _tempTestNode->addMesh(box);
    s->root3D()->addChild(_tempTestNode);

    SLMat4f test;
    test.translate(10, 0, 0);
    test.rotate(40, 0, 1, 0);
    test.translate(-10, 0, 0);

    SLMat4f modelTransform;
    modelTransform.identity();
    modelTransform.rotate(30, 0, 1, 0);
    modelTransform.rotate(30, 1, 0, 0);
    
    _tempTestNode->om(modelTransform * test);

	_currentActiveCamIndex = _numberOfPlayerCams + 1;
}
//-----------------------------------------------------------------------------
/*!
handles data transfer before repaint the scene
*/
void VRSceneView::preDraw()
{
	SLScene* s = SLScene::current;

	RakNet::BitStream* bsIn = NULL;
	bool dataReceived = false;
	if (_networkClient->isConnected())
	{
		dataReceived = _networkClient->receiveServerData(&bsIn);
	}

	if(_clientType == VR_CLIENT_TYPE_PLAYER)
	{
		_kinect->refreshKinectData();
		_vrPerson.addFrame(_kinect->getKinectData());

		//cout << "Position: " << floor(_kinect->getKinectData()->position.z * 10000) << endl;

		// retrieve the left and right hand position of the currently active mesh skeleton
		SLVec3f leftHand, rightHand;
		_gameScene->rebuildLocalFigure(_vrPerson.getData());
		_gameScene->getLocalPersonHandPos(leftHand, rightHand);

		if(_networkClient->isConnected())
		{
			_networkClient->sendClientData(*_vrPerson.getData(), leftHand, rightHand);
		}
	}

	if(dataReceived){
		_gameScene->updateFromWorldState(bsIn, _vrPerson.getDefaultData());
	}

	if(_networkDataStorage.collectNetworkData)
		this->collectNetworkData();

	delete bsIn;

	if (this->_isObserverCamLoopActive)
	{
		if (_camLoopTimer.getElapsedTimeInSec() - _lastCamChangeTime > CAMERA_LOOP_TIMESPAN_SEC)
		{
			_currentActiveCamIndex = (_currentActiveCamIndex + 1) % (_numberOfPlayerCams + 2);
			if (_currentActiveCamIndex < _numberOfPlayerCams)
			{
				_gameScene->activateCam(_currentActiveCamIndex);
			}
			else
			{
				SLScene* s = SLScene::current;
				if (_currentActiveCamIndex == _numberOfPlayerCams)
				{
					camera(_freeCam);
				}

				if (_currentActiveCamIndex == _numberOfPlayerCams+1)
				{
					camera(_freeCam2);
				}
			}
			_lastCamChangeTime = _camLoopTimer.getElapsedTimeInSec();
		}
	}
}

//-----------------------------------------------------------------------------
/*!
virtual room key event handle
*/
SLbool VRSceneView::onKeyPress(const SLKey key, const SLKey mod)
{/*/
	if(key == 44)
	{
		_vrPerson.scaleUp();
		_oculus.eyeSeperation(_oculus.eyeSeparation()*1.1f);
	}else
	if(key == 46){
		_vrPerson.scaleDown();
		_oculus.eyeSeperation(_oculus.eyeSeparation()*0.9f);
	}else*/
	
	// TODO: clean up this mess below
	if(key == 'L'){
		_networkClient->sendPingPacket();
	}
	else
	if(key == 'Z'){
		_networkDataStorage.collectNetworkData = true;
		_networkDataStorage.currentDataCollectingIndex = 0;
		_networkDataStorage.collectingStartTime = _networkClient->getTimer()->getElapsedTimeInSec();
		_networkClient->sendPingPacket();
	}
	else
	if(_clientType == VR_CLIENT_TYPE_OBSERVER)
	{
		// switch camera perspective
		if(key == 'O'){
			SLScene* s = SLScene::current;
			if(camera()->projection() == stereoSideBySideD)
				camera()->projection(monoPerspective);
			else
				camera()->projection(stereoSideBySideD);
		}

		// activate player camera
		int playerKey = 48;
		for(int i = 0; i < _numberOfPlayerCams; i++)
		{
			++playerKey;
			if(key == playerKey)
			{
				_gameScene->activateCam(i);
				_isObserverCamLoopActive = false;
				_currentActiveCamIndex = i;
			}
		}
		
		// activate free camera
		++playerKey;
		if(key == playerKey){
			SLScene* s = SLScene::current;
			camera(_freeCam);
			_isObserverCamLoopActive = false;
			_currentActiveCamIndex = _numberOfPlayerCams + 1;
		}
		
		// activate loop over the cameras
		++playerKey;
		if(key == playerKey){
			SLScene* s = SLScene::current;
			camera(_freeCam2);
			_isObserverCamLoopActive = false;
			_currentActiveCamIndex = _numberOfPlayerCams + 2;
		}
		
		// activate loop over the cameras
		++playerKey;
		if(key == playerKey){
			if(!this->_isObserverCamLoopActive)
			{
				_currentActiveCamIndex = 0;
				if (_numberOfPlayerCams > 0)
				{
					_gameScene->activateCam(_currentActiveCamIndex);
				}
				else
				{
					SLScene* s = SLScene::current;
					camera(_freeCam);
				}
				_isObserverCamLoopActive = true;
				_lastCamChangeTime = _camLoopTimer.getElapsedTimeInSec();
			}
		}	
	}/**/
	
	return SLSceneView::onKeyPress(key, mod);
}
//-----------------------------------------------------------------------------
/*!
virtual room key event handle
*/
void VRSceneView::collectNetworkData()
{
	_networkDataStorage.lastDataPacketReceived[_networkDataStorage.currentDataCollectingIndex] = _networkClient->getTimer()->getElapsedTimeInSec() - _networkClient->lastDataPacketReceived();
	_networkDataStorage.numberOfReceivedDataPackets[_networkDataStorage.currentDataCollectingIndex] = _networkClient->numberOfReceivedDataPackets();

	// collect ping data
	if(_networkClient->lastPingtime() > 0){
		_networkDataStorage.pingData[_networkDataStorage.currentDataCollectingIndex] = _networkClient->lastPingtime();
		_networkClient->sendPingPacket();
	}else{
		_networkDataStorage.pingData[_networkDataStorage.currentDataCollectingIndex] = 0;
	}

	++_networkDataStorage.currentDataCollectingIndex;

	// stop collecting
	if(_networkDataStorage.currentDataCollectingIndex == NETWORK_DATA_SAMPLE_COUNT){
		_networkDataStorage.collectNetworkData = false;
		double collectingTime = _networkClient->getTimer()->getElapsedTimeInSec() - _networkDataStorage.collectingStartTime;
		ofstream dataFile;
		dataFile.open ("network_data.csv");
		dataFile << "last data packet;number of packets;ping time" << endl;
		for(int i = 0; i < NETWORK_DATA_SAMPLE_COUNT; i++){
			dataFile << _networkDataStorage.lastDataPacketReceived[i];
			dataFile << ";" << _networkDataStorage.numberOfReceivedDataPackets[i];
			dataFile << ";" << _networkDataStorage.pingData[i];

			// do statistic
			if(i == 0)
				dataFile << ";;FPS:;" << NETWORK_DATA_SAMPLE_COUNT / collectingTime;

			dataFile << endl;
		}			
		dataFile.close();
	}
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/*!
returns a new scene view
*/
SLSceneView* getSceneView()
{
	return new VRSceneView;
}