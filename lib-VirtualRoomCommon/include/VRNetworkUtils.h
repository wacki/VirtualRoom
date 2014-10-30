//#############################################################################
//  File:      VRNetworkUtils.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VRNETWORKUTILS_H
#define VRNETWORKUTILS_H

#include "MessageIdentifiers.h"
#include "RakNetTypes.h"
#include "PacketPriority.h"
#include "RakPeer.h"

// SLProject
#include <stdafx.h>					// precompiled headers with math stuff

#include "VRBaseMessage.h"
#include "VRTypes.h"

namespace VirtualRoom
{
	namespace Network
	{
		//-----------------------------------------------------------------------------
		//! defines the different packet types of all messages
		enum IDENTIFIERS
		{
			ID_CONNECTING_QUESTION=ID_USER_PACKET_ENUM+1,
			ID_CONNECTING_ANSWER=ID_CONNECTING_QUESTION+1,
			ID_DATA_CLIENT=ID_CONNECTING_ANSWER+1,
			ID_DATA_SERVER=ID_DATA_CLIENT+1,
			ID_PING_PACKET=ID_DATA_SERVER+1
		};
		//-----------------------------------------------------------------------------
		class BaseMessage;										// Foreward declaration
		//-----------------------------------------------------------------------------
		unsigned char GetPacketIdentifier (RakNet::Packet *p);
		//-----------------------------------------------------------------------------
		void SendStructMessage(RakNet::RakPeerInterface* peer, RakNet::SystemAddress& address, BaseMessage& message, PacketPriority priority, PacketReliability reliability);
		//-----------------------------------------------------------------------------
		/*! 
		template function which gets the data of a packet and creates an object of
		the appopriate derived message class
		*/
		template<class T> T* GetStructFromDataMessage(RakNet::Packet* packet)
		{
			RakNet::BitStream bsIn(packet->data, packet->length, false);
			T* result = new T(&bsIn);
			return result;
		}
		//-----------------------------------------------------------------------------
	}
}
//-----------------------------------------------------------------------------
#endif