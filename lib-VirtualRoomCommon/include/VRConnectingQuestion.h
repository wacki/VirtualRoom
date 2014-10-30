//#############################################################################
//  File:      VRConnectingQuestion.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VRCONNECTINGQUESTION_H
#define VRCONNECTINGQUESTION_H

#include "VRNetworkUtils.h"

namespace VirtualRoom
{
	namespace Network
	{
		//-----------------------------------------------------------------------------
		//! packet with the request from the client to the server for a connection
		/*!      
		represents a packet with the request of a client (with its type) at the beginning
		of a new connection
		*/
		class ConnectingQuestion : public BaseMessage
		{
		public:
			VR_CLIENT_TYPE				_clientType;				//!< type of the client

			ConnectingQuestion			(double _timeStamp, VR_CLIENT_TYPE clientType) : BaseMessage(ID_CONNECTING_QUESTION, _timeStamp), _clientType(clientType) { };
			ConnectingQuestion			(RakNet::BitStream* bsIn);
			~ConnectingQuestion			() { };
	
			void serialize				(RakNet::BitStream* bsOut);
		};
	}
}
//-----------------------------------------------------------------------------
#endif