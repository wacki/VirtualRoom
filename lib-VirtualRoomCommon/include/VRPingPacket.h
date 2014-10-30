//#############################################################################
//  File:      VRPingPacket.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include "VRNetworkUtils.h"

#ifndef VRPINGPACKET_H
#define VRPINGPACKET_H

namespace VirtualRoom
{
	namespace Network
	{
		//-----------------------------------------------------------------------------
		//! empty ping packet just requesting an answer of the same of type
		/*!      
		when a empty ping packet is sent from the client to the server, the latter
		will respond sending also an empty ping packet to the client
		*/
		class PingPacket : public BaseMessage
		{
		public:
			PingPacket				(double _timeStamp) : BaseMessage(ID_PING_PACKET, _timeStamp) { };
			PingPacket				(RakNet::BitStream* bsIn) : BaseMessage(bsIn)	{ };
			~PingPacket				() { };

			void serialize			(RakNet::BitStream* bsOut) { };
		};
	}
}
//-----------------------------------------------------------------------------
#endif