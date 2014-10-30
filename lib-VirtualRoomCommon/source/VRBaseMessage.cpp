//#############################################################################
//  File:      VRBaseMessage.cpp
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include "VRBaseMessage.h"

using namespace VirtualRoom::Network;

//-----------------------------------------------------------------------------
BaseMessage::BaseMessage(RakNet::BitStream* bsIn)
{
	bsIn->Read(this->_typeId);
	bsIn->Read(this->_timeStamp);
}
//-----------------------------------------------------------------------------
/*! 
return the timestamp within the packet
*/
double BaseMessage::getTimeStamp()
{
	return this->_timeStamp;
}
//-----------------------------------------------------------------------------
/*! 
return the type of the packet
*/
RakNet::MessageID BaseMessage::getTypeId()
{
	return this->_typeId;
}
//-----------------------------------------------------------------------------
/*! 
writes the content of the packet into the given bit stream. Thus the order of 
writing data is important. First the type, then the timestamp is written into
the bit stream. After those values the effictive data of the message packet
implementation are written by the implementing classes themself
*/
void BaseMessage::serializeMessage(RakNet::BitStream* bsOut)
{
	if (bsOut == NULL)
		bsOut = new RakNet::BitStream();

	bsOut->Write(this->_typeId);
	bsOut->Write(this->_timeStamp);

	this->serialize(bsOut);
}
//-----------------------------------------------------------------------------