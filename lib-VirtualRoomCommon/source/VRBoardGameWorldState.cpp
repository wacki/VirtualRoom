//#############################################################################
//  File:      VRBoardGameWorldState.cpp
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include "VRBoardGameWorldState.h"

using namespace VirtualRoom;

//-----------------------------------------------------------------------------
BoardGameWorldState::BoardGameWorldState(double timeStamp, SLint maxPlayers) : WorldState(timeStamp, maxPlayers)
{
	_resetBoxColors = new SLCol4f[_maxPlayers];
}
//-----------------------------------------------------------------------------
BoardGameWorldState::BoardGameWorldState(RakNet::BitStream* bsIn)
	: WorldState(bsIn)
{
	bsIn->Read(_gameState);
	bsIn->Read(_actPlayerId);

	_resetBoxColors = new SLCol4f[_maxPlayers];

	// read ResetBoxColors data
	for(SLint i = 0; i < _maxPlayers; i++)
		bsIn->Read(_resetBoxColors[i]);
}
//-----------------------------------------------------------------------------
BoardGameWorldState::~BoardGameWorldState()
{
	delete[] _resetBoxColors;
}
//-----------------------------------------------------------------------------
/*! 
writes the world state data first, then the additional board game application-
specific data into the given bit stream in the correct order
*/
void BoardGameWorldState::serialize(RakNet::BitStream* bsOut)
{
	WorldState::serialize(bsOut);
	bsOut->Write(_gameState);
	bsOut->Write(_actPlayerId);

	// send ResetBoxColors data
	for(SLint i = 0; i < _maxPlayers; i++)
		bsOut->Write(_resetBoxColors[i]);
}
//-----------------------------------------------------------------------------