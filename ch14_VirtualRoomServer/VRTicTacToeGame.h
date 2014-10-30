//#############################################################################
//  File:      VRTicTacToeGame.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VRTICTACTOEGAME_H
#define VRTICTACTOEGAME_H

#include "VRTicTacToeConstants.h"
#include "VRBoardGame.h"

namespace VirtualRoom
{
	namespace Game
	{
		//-----------------------------------------------------------------------------
		//! tic tac toe game
		/*!      
		extends the basic board game for the tic tac toe game
		*/
		//-----------------------------------------------------------------------------
		class TicTacToeGame : public BoardGame
		{
		public:
			TicTacToeGame						();
			~TicTacToeGame						() { };
			
			void updateGameState				(int indexLastSelectedX, int indexLastSelectedY, int indexLastSelectedZ);
			WorldState* getBoardState			(SLTimer &timer, SLint maxPlayers);
			BoardPlayField* checkHit			(SLVec3f& effectivePosLeftHand, SLVec3f& effectivePosRightHand, int& indexLastSelectedX, int& indexLastSelectedY, int& indexLastSelectedZ);

		private:
			SLuint calculateWinner				(SLuint board);
		};
	}
}
//-----------------------------------------------------------------------------
#endif