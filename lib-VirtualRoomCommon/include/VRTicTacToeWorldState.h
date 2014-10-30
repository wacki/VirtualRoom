//#############################################################################
//  File:      VRTicTacToeWorldState.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VRTICTACTOEWORLDSTATE_H
#define VRTICTACTOEWORLDSTATE_H

#include "VRBoardGameWorldState.h"
#include "VRTicTacToeConstants.h"

namespace VirtualRoom
{
	//-----------------------------------------------------------------------------
	//! world state for the board game 'tic tac toe'
	/*!      
	represents a worldstate for board game application 'tic tac toe'
	*/
	class TicTacToeWorldState : public BoardGameWorldState
	{
	public:
		SLCol4f							_fieldColors[TICTACTOE_NUMBER_OF_FIELDS];			//!< color of every field on the tic tac toe board

		TicTacToeWorldState				(double timeStamp, SLint maxPlayers);
		TicTacToeWorldState				(RakNet::BitStream* bsIn);
		~TicTacToeWorldState			();

		virtual void serialize			(RakNet::BitStream* bsOut);	
	};
}
//-----------------------------------------------------------------------------
#endif