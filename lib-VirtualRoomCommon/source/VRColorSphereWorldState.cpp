//#############################################################################
//  File:      VRColorSphereWorldState.cpp
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include "VRColorSphereWorldState.h"

using namespace VirtualRoom;

//-----------------------------------------------------------------------------
ColorSphereWorldState::ColorSphereWorldState(RakNet::BitStream* bsIn)
	: WorldState(bsIn)
{
	bsIn->Read(_sphereColor);
}
//-----------------------------------------------------------------------------
/*! 
writes the world state data first, then the additional turn table application-
specific data into the given bit stream in the correct order
*/
void ColorSphereWorldState::serialize(RakNet::BitStream* bsOut)
{
	WorldState::serialize(bsOut);
	bsOut->Write(_sphereColor);
}
//-----------------------------------------------------------------------------