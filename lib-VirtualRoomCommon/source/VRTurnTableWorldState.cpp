//#############################################################################
//  File:      VRTurnTableWorldState.cpp
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include "VRTurnTableWorldState.h"

using namespace VirtualRoom;

//-----------------------------------------------------------------------------
TurnTableWorldState::TurnTableWorldState(RakNet::BitStream* bsIn)
	: WorldState(bsIn)
{
	bsIn->Read(_rotation);
}
//-----------------------------------------------------------------------------
/*! 
writes the world state data first, then the additional turn table application-
specific data into the given bit stream in the correct order
*/
void TurnTableWorldState::serialize(RakNet::BitStream* bsOut)
{
	WorldState::serialize(bsOut);
	bsOut->Write(_rotation);
}
//-----------------------------------------------------------------------------