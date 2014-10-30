//#############################################################################
//  File:      VRConnectFourWorldState.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VRCONNECTFOURWORLDSTATE_H
#define VRCONNECTFOURWORLDSTATE_H

#include "VRBoardGameWorldState.h"
#include "VRConnectFourConstants.h"

namespace VirtualRoom
{
	//-----------------------------------------------------------------------------
	//! world state for the board game 'connect four'
	/*!      
	represents a worldstate for board game application 'connect four'
	*/
	class ConnectFourWorldState : public BoardGameWorldState
	{
	public:
		SLCol4f							_cylinderColors[CONNECTFOUR_NUMBER_OF_CYLINDERS];	//!< color of every cylinder on the board
		SLint							_numberOfSpheres;									//!< current number of spheres on the board (with those in motion)
		SLCol4f*						_sphereColors;										//!< color for each sphere
		SLVec3f*						_spherePositions;									//!< current position for each sphere
		SLbool*							_sphereIsWinner;									//!< true for all winning spheres, false otherwise

		ConnectFourWorldState			(double timeStamp, SLint maxPlayers);
		ConnectFourWorldState			(RakNet::BitStream* bsIn);
		~ConnectFourWorldState			();

		virtual void serialize			(RakNet::BitStream* bsOut);	
	};
}
//-----------------------------------------------------------------------------
#endif