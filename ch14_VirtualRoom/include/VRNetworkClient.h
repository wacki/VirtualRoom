//#############################################################################
//  File:      VRNetworkClient.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VRNETWORKCLIENT_H
#define VRNETWORKCLIENT_H

// RakNet
#include "RakPeerInterface.h"
#include "RakNetTypes.h"

// VirtualRoom Network
#include "VRConnectingAnswer.h"
#include "VRConnectingQuestion.h"
#include "VRClientData.h"
#include "VRWorldState.h"
#include "VRPingPacket.h"

namespace VirtualRoom
{
	#define SERVER_PORT 60000
	
	//-----------------------------------------------------------------------------
	//! network client
	/*!      
	manages the communication to the server
	*/
	//-----------------------------------------------------------------------------
	class NetworkClient
	{
	public:
		NetworkClient(string serverAddress, VR_CLIENT_TYPE clientType) : _serverAddress(serverAddress), _clientType(clientType), _pingStartTime(-1), _lastPingTime(-1) { }
		~NetworkClient();

		void						connect						();
		void						waitForConnect				();
		void						sendConnectingQuestion		();
		void						waitForConnectingAnswer		(Network::ConnectingAnswer** connectingAnswer);
		bool						receiveServerData			(RakNet::BitStream** bsIn);
		void						sendClientData				(SkeletonData& data, SLVec3f& leftHand, SLVec3f& rightHand);
		bool						isConnected					();
		void						sendPingPacket				();

		double						lastDataPacketReceived		() { return _lastDataPacketReceived; }
		SLint						numberOfReceivedDataPackets () { return _numberOfReceivedDataPackets; }
		double						lastPingtime				() { return _lastPingTime; }
		SLTimer*					getTimer					() { return &_timer; }	

	private:
		RakNet::RakPeerInterface*	_peer;							//!< raknet interface
		RakNet::SystemAddress		_systemaddress;					//!< raknet system address
		bool						_connected;						//!< is connected to the server
		string						_serverAddress;					//!< server network address
		VR_CLIENT_TYPE				_clientType;					//!< client type
		SLTimer						_timer;							//!< timer
		double						_pingStartTime;					//!< ping sended time
		double						_lastPingTime;					//!< last ping time
		double						_lastDataPacketReceived;		//!< data packet information
		SLint						_numberOfReceivedDataPackets;	//!< number of received data packets
	};
}
//-----------------------------------------------------------------------------
#endif