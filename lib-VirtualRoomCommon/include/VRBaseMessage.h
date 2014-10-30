//#############################################################################
//  File:      VRBaseMessage.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VRBASEMESSAGE_H
#define VRBASEMESSAGE_H

#include "VRNetworkUtils.h"

namespace VirtualRoom
{
	namespace Network
	{
		//-----------------------------------------------------------------------------
		//! base class for any message packet
		/*!      
		represents the base class for any implemented message packet. Each packet has
		a type identifier as well as a timestamp indicating when the packet was created.
		the serializeMessage function will first write the type and the timestamp into
		the bit stream. Then the full virtual function serialize will be called. For 
		deserialization the appropriate constructor has to be called.
		*/
		class BaseMessage
		{
		public:
			BaseMessage						(RakNet::MessageID typeId, double timeStamp) : _typeId(typeId), _timeStamp(timeStamp) { };
			BaseMessage						(RakNet::BitStream* bsIn);
			virtual ~BaseMessage			() { };

			double getTimeStamp				();
			RakNet::MessageID getTypeId		();
			void serializeMessage			(RakNet::BitStream* bsOut);

		protected:
			RakNet::MessageID				_typeId;				//!< type of any message packet
			double							_timeStamp;				//!< timestamp at creation of any message packet

			virtual void serialize			(RakNet::BitStream* bsOut) = 0;
		};
	}
}
//-----------------------------------------------------------------------------
#endif