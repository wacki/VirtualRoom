//#############################################################################
//  File:      VRNetworkUtils.cpp
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include "VRNetworkUtils.h"

//-----------------------------------------------------------------------------
/*! 
returns the type of any packet as the first byte in the stream of the packet
*/
unsigned char VirtualRoom::Network::GetPacketIdentifier(RakNet::Packet *p)
{
	if (p==0)
		return 255;

	return (unsigned char) p->data[0];
}
//-----------------------------------------------------------------------------
/*! 
sends any message object (as a derived instance of BaseMessage)
*/
void VirtualRoom::Network::SendStructMessage(RakNet::RakPeerInterface* peer, RakNet::SystemAddress& address, BaseMessage& message, PacketPriority priority, PacketReliability reliability)
{
	RakNet::BitStream* bsOut = new RakNet::BitStream();

	message.serializeMessage(bsOut);

	peer->Send(bsOut, priority, reliability, 0, address, false);

	delete bsOut;
}
//-----------------------------------------------------------------------------