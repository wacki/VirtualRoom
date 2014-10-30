//#############################################################################
//  File:      VRServer.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VRSERVER_H
#define VRSERVER_H

// RakNet
#include "RakPeerInterface.h"
#include "BitStream.h"
#include "RakNetTypes.h"

// VirtualRoom Network
#include "VRConnectingAnswer.h"
#include "VRConnectingQuestion.h"
#include "VRPingPacket.h"
#include "VRClientData.h"
#include "VRWorldState.h"

namespace VirtualRoom
{
	#define MAX_OBSERVERS 6
	#define SERVER_PORT 60000
	#define REFRESH_RATE 16.0		// in milli - seconds
	#define DISTANCE_TO_OBJECT 1


	//-----------------------------------------------------------------------------
	//! player state
	struct PlayerState
	{
		SLint*	clientIndex;
		SLint	nextFreeSlot;
		SLint	activePlayerCount;
	};
	//-----------------------------------------------------------------------------
	//! server base class
	/*!      
	the server controller class manages the communication with raknet
	*/
	//-----------------------------------------------------------------------------
	class Server
	{
	public:
		Server							(SLint maxPersons, VR_GAME_TYPE gameType);
		virtual ~Server					();

		void connect					();
		void run						();
		void stop						();

	protected:
		SLint							_maxPlayers;			//!< maximum players
		SLint							_maxClients;			//!< maximum clients
		SLbool							_running;				//!< is running
		RakNet::RakPeerInterface*		_peer;					//!< raknet interface
		SLMat4f*						_personMatrices;		//!< start positions matrices
		VR_MODEL*						_personModels;			//!< models the clients use
		SkeletonData*					_skeletonDatas;			//!< storage of skeleton data
		SLVec3f*						_leftHandPos;			//!< left hand position in world space
		SLVec3f*						_rightHandPos;			//!< right hand position in world space
		SLbool*							_hasSkeletonData;		//!< has skeleton data
		SLbool*							_requestPingAnswer;		//!< has ping request
		VR_CLIENT_TYPE*					_sendDataToClientIndex;	//!< if a client needs data
		PlayerState*					_playerStates;			//!< player states
		VR_GAME_TYPE					_gameType;				//!< server game type
		SLTimer							_timer;					//!< timer

		void handlePacket					(RakNet::Packet* packet);
		void requestStartAddress			(unsigned short index, RakNet::Packet* packet);
		void receiveDataFromClient			(unsigned short index, RakNet::Packet* packet);
		void sendRequestedPings				();
		void sendDataToAllClients			();
		void disconnectClient				(unsigned short index);

		virtual void simulate				() {}
		virtual WorldState* getWorldState	() { return new WorldState(_timer.getElapsedTimeInMilliSec(), _maxPlayers); }
	};
}
//-----------------------------------------------------------------------------
#endif