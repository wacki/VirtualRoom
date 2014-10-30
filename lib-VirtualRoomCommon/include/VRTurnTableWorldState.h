//#############################################################################
//  File:      VRTurnTableWorldState.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VRTURNTABLEWORLDSTATE_H
#define VRTURNTABLEWORLDSTATE_H

#include "VRWorldState.h"

namespace VirtualRoom
{
	//-----------------------------------------------------------------------------
	//! world state for the turn table application
	/*!      
	represents a worldstate for the turn table application
	*/
	class TurnTableWorldState : public WorldState
	{
	public:
		SLQuat4f _rotation;			//!< Current rotation of the table with its mesh on the top

		TurnTableWorldState			(double timeStamp, SLint maxPlayers) : WorldState(timeStamp, maxPlayers) {}
		TurnTableWorldState			(RakNet::BitStream* bsIn);

		virtual void serialize		(RakNet::BitStream* bsOut);	
	};
}
//-----------------------------------------------------------------------------
#endif