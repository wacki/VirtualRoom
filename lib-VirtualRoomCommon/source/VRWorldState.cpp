//#############################################################################
//  File:      VRWorldState.cpp
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include "VRWorldState.h"

using namespace VirtualRoom;

//-----------------------------------------------------------------------------
WorldState::WorldState(double timeStamp, SLint maxPlayers)
	: BaseMessage(Network::ID_DATA_SERVER, timeStamp), _maxPlayers(maxPlayers)
{
	_playerData		 = new SkeletonData[maxPlayers];
	_hasSkeletonData = new SLbool[maxPlayers];
}
//-----------------------------------------------------------------------------
WorldState::WorldState(RakNet::BitStream* bsIn)
	: BaseMessage(bsIn)
{
	bsIn->Read(_maxPlayers);
	_hasSkeletonData = new SLbool[_maxPlayers];
	_playerData = new SkeletonData[_maxPlayers];

	// read has data
	for(SLint i = 0; i < _maxPlayers; i++)
		bsIn->Read(_hasSkeletonData[i]);
	
	// read player data
	for(SLint i = 0; i < _maxPlayers; i++)
		if(_hasSkeletonData[i])
			bsIn->Read(_playerData[i]);
}
//-----------------------------------------------------------------------------
WorldState::~WorldState()
{
	delete[] _playerData;
	delete[] _hasSkeletonData;
};
//-----------------------------------------------------------------------------
/*! 
writes the world state data into the given bit stream in the correct order
*/
void WorldState::serialize(RakNet::BitStream* bsOut)
{
	bsOut->Write(_maxPlayers);
	// send has data
	for(SLint i = 0; i < _maxPlayers; i++)
		bsOut->Write(_hasSkeletonData[i]);
	
	// send player data
	for(SLint i = 0; i < _maxPlayers; i++)
		if(_hasSkeletonData[i])
			bsOut->Write(_playerData[i]);
		
}
//-----------------------------------------------------------------------------