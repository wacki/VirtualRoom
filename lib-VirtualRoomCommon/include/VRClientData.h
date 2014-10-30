//#############################################################################
//  File:      VRClientData.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VRCLIENTDATA_H
#define VRCLIENTDATA_H

#include "VRNetworkUtils.h"

namespace VirtualRoom
{
	namespace Network
	{
		//-----------------------------------------------------------------------------
		//! packet with data sent from the client to the server
		/*!      
		represents a packet with the skeleton data sent from a client to the server
		*/
		class ClientData : public BaseMessage
		{
		public:
			SkeletonData				_data;			//!< skeleton data captured by the kinect on the client
			SLVec3f						_handPosition[VR_HAND_COUNT];

			ClientData					(double _timeStamp) : BaseMessage(ID_DATA_CLIENT, _timeStamp) { };
			ClientData					(RakNet::BitStream* bsIn);
			~ClientData					() { };

			void serialize				(RakNet::BitStream* bsOut);
		};
	}
}
//-----------------------------------------------------------------------------
#endif