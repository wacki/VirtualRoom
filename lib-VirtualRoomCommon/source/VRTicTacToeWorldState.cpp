//#############################################################################
//  File:      VRTicTacToeWorldState.cpp
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include "VRTicTacToeWorldState.h"

using namespace VirtualRoom;

//-----------------------------------------------------------------------------
TicTacToeWorldState::TicTacToeWorldState(double timeStamp, SLint maxPlayers) : BoardGameWorldState(timeStamp, maxPlayers)
{

}
//-----------------------------------------------------------------------------
TicTacToeWorldState::TicTacToeWorldState(RakNet::BitStream* bsIn)
	: BoardGameWorldState(bsIn)
{
	bsIn->Read(_fieldColors);
}
//-----------------------------------------------------------------------------
TicTacToeWorldState::~TicTacToeWorldState()
{

}
//-----------------------------------------------------------------------------
/*! 
writes the borad game world state data first, then the additional 'tic tac toe'
application-specific data into the given bit stream in the correct order
*/
void TicTacToeWorldState::serialize(RakNet::BitStream* bsOut)
{
	BoardGameWorldState::serialize(bsOut);
	bsOut->Write(_fieldColors);
}
//-----------------------------------------------------------------------------