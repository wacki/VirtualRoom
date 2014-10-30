//#############################################################################
//  File:      VRConnectingAnswer.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VRCONNECTINGANSWER_H
#define VRCONNECTINGANSWER_H

#include "VRNetworkUtils.h"

namespace VirtualRoom
{
	namespace Network
	{
		//-----------------------------------------------------------------------------
		//! packet with the answer from the server to the client on a connection request
		/*!      
		represents a packet with all the data needed by the client at the beginning of
		a new connection. Thus the server will send a ConnectingAnswer message packet
		to the client. There are all the information letting the client build 
		*/
		class ConnectingAnswer : public BaseMessage
		{
		public:
			SLint					_maxPlayers;			//!< maximum number of players for displaying them in the virtual room
			SLint					_playerIndex;			//!< own player index for setting the own kinect data directly
			SLMat4f*				_initMat;				//!< initial position matrix of every person in the virtual room
			VR_MODEL*				_initModels;			//!< initial models for the clients
			VR_GAME_TYPE			_gameType;				//!< game type for loading appropriate scene

			ConnectingAnswer		(double _timeStamp, SLint maxPlayers, SLint playerIndex, VR_GAME_TYPE gameType) : BaseMessage(ID_CONNECTING_ANSWER, _timeStamp), _maxPlayers(maxPlayers), _playerIndex(playerIndex), _gameType(gameType) { };
			ConnectingAnswer		(RakNet::BitStream* bsIn);
				
			void serialize			(RakNet::BitStream* bsOut);
		};
	}
}
//-----------------------------------------------------------------------------
#endif