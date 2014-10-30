//#############################################################################
//  File:      VRColorSphereWorldState.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VRCOLORSPHEREWORLDSTATE_H
#define VRCOLORSPHEREWORLDSTATE_H

#include "VRWorldState.h"

namespace VirtualRoom
{
	//-----------------------------------------------------------------------------
	//! world state for the color sphere application
	/*!      
	represents a worldstate for the color sphere application
	*/
	class ColorSphereWorldState : public WorldState
	{
	public:
		SLCol4f		_sphereColor;			//!< Current color of the sphere

		ColorSphereWorldState		(double timeStamp, SLint maxPlayers) : WorldState(timeStamp, maxPlayers) { };
		ColorSphereWorldState		(RakNet::BitStream* bsIn);

		virtual void serialize		(RakNet::BitStream* bsOut);	
	};
}
//-----------------------------------------------------------------------------
#endif