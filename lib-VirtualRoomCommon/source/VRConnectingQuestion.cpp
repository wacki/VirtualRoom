//#############################################################################
//  File:      VRConnectingQuestion.cpp
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include "VRConnectingQuestion.h"

using namespace VirtualRoom::Network;

//-----------------------------------------------------------------------------
ConnectingQuestion::ConnectingQuestion(RakNet::BitStream* bsIn) : BaseMessage(bsIn)
{
	bsIn->Read(this->_clientType);
}
//-----------------------------------------------------------------------------
/*! 
writes the client type (connecting question data) into the given bit stream
*/
void ConnectingQuestion::serialize(RakNet::BitStream* bsOut)
{
	bsOut->Write(this->_clientType);
}
//-----------------------------------------------------------------------------