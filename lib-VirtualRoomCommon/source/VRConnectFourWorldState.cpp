//#############################################################################
//  File:      VRConnectFourWorldState.cpp
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include "VRConnectFourWorldState.h"

using namespace VirtualRoom;

//-----------------------------------------------------------------------------
ConnectFourWorldState::ConnectFourWorldState(double timeStamp, SLint maxPlayers) : BoardGameWorldState(timeStamp, maxPlayers)
{
	_numberOfSpheres = 0;
	_sphereColors = NULL;
	_spherePositions = NULL;
}
//-----------------------------------------------------------------------------
ConnectFourWorldState::ConnectFourWorldState(RakNet::BitStream* bsIn)
	: BoardGameWorldState(bsIn)
{
	bsIn->Read(_cylinderColors);
	bsIn->Read(_numberOfSpheres);

	_sphereColors = new SLCol4f[_numberOfSpheres];
	_spherePositions = new SLVec3f[_numberOfSpheres];
	_sphereIsWinner = new SLbool[_numberOfSpheres];

	// read sphere data
	for(SLint i = 0; i < _numberOfSpheres; i++)
	{
		bsIn->Read(_sphereColors[i]);
		bsIn->Read(_spherePositions[i]);
		bsIn->Read(_sphereIsWinner[i]);
	}
}
//-----------------------------------------------------------------------------
ConnectFourWorldState::~ConnectFourWorldState()
{
	if (_sphereColors != NULL)
	{
		delete[] _sphereColors;
	}
	if (_spherePositions != NULL)
	{
		delete[] _spherePositions;
	}
	if (_sphereIsWinner != NULL)
	{
		delete[] _sphereIsWinner;
	}
}
//-----------------------------------------------------------------------------
/*! 
writes the borad game world state data first, then the additional 'connect four'
application-specific data into the given bit stream in the correct order
*/
void ConnectFourWorldState::serialize(RakNet::BitStream* bsOut)
{
	BoardGameWorldState::serialize(bsOut);
	bsOut->Write(_cylinderColors);
	bsOut->Write(_numberOfSpheres);

	// send sphere data
	for(SLint i = 0; i < _numberOfSpheres; i++)
	{
		bsOut->Write(_sphereColors[i]);
		bsOut->Write(_spherePositions[i]);
		bsOut->Write(_sphereIsWinner[i]);
	}
}
//-----------------------------------------------------------------------------