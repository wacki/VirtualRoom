//#############################################################################
//  File:      VRBoardGameWorldState.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VRBOARDGAMEWORLDSTATE_H
#define VRBOARDGAMEWORLDSTATE_H

#include "VRWorldState.h"
#include "VRBoardGameConstants.h"

namespace VirtualRoom
{
	//-----------------------------------------------------------------------------
	//! base world state of any board game
	/*!      
	represents a worldstate for any type of board game
	*/
	class BoardGameWorldState : public WorldState
	{
	public:
		VR_BOARDGAME_STATE				_gameState;				//!< Actual state of the board game
		SLint							_actPlayerId;			//!< ID of the player which is in turn 
		SLCol4f*						_resetBoxColors;		//!< Actual colors of the reset boxes

		BoardGameWorldState				(double timeStamp, SLint maxPlayers);
		BoardGameWorldState				(RakNet::BitStream* bsIn);
		virtual ~BoardGameWorldState	();

		virtual void serialize			(RakNet::BitStream* bsOut);	
	};
}
//-----------------------------------------------------------------------------
#endif