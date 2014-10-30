//#############################################################################
//  File:      VRClientData.cpp
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include "VRClientData.h"

using namespace VirtualRoom::Network;

//-----------------------------------------------------------------------------
ClientData::ClientData(RakNet::BitStream* bsIn) : BaseMessage(bsIn)
{
	bsIn->Read(this->_data);	
	bsIn->Read(this->_handPosition);
}
//-----------------------------------------------------------------------------
/*! 
writes the skeleton data into the given bit stream
*/
void ClientData::serialize(RakNet::BitStream* bsOut)
{
	bsOut->Write(this->_data);
	bsOut->Write(this->_handPosition);
}
//-----------------------------------------------------------------------------