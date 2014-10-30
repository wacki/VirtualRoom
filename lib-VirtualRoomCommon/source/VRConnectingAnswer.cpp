//#############################################################################
//  File:      VRConnectingAnswer.cpp
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include "VRConnectingAnswer.h"

using namespace VirtualRoom::Network;
using namespace VirtualRoom;

//-----------------------------------------------------------------------------
ConnectingAnswer::ConnectingAnswer(RakNet::BitStream* bsIn) : BaseMessage(bsIn)
{
	bsIn->Read(_gameType);
	bsIn->Read(_playerIndex);
	bsIn->Read(_maxPlayers);
	_initMat = new SLMat4f[_maxPlayers];
	for(SLint i = 0; i < _maxPlayers; i++)
		bsIn->Read(_initMat[i]);
	_initModels = new VR_MODEL[_maxPlayers];
	for(SLint i = 0; i < _maxPlayers; i++)
		bsIn->Read(_initModels[i]);
}
//-----------------------------------------------------------------------------
/*! 
writes all the connecting answer data, which are needed by the client,
into the given bit stream
*/
void ConnectingAnswer::serialize(RakNet::BitStream* bsOut)
{
	bsOut->Write(_gameType);
	bsOut->Write(_playerIndex);
	bsOut->Write(_maxPlayers);
	for(SLint i = 0; i < _maxPlayers; i++)
		bsOut->Write(_initMat[i]);
	for(SLint i = 0; i < _maxPlayers; i++)
		bsOut->Write(_initModels[i]);
}
//-----------------------------------------------------------------------------