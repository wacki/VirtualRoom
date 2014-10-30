//#############################################################################
//  File:      VRTicTacToeGameScene.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VRTICTACTOCGAMESCENE_H
#define VRTICTACTOCGAMESCENE_H

#include "VRBoardGameScene.h"
#include "VRTicTacToeWorldState.h"
#include "VRTicTacToeConstants.h"
#include "SLBox.h"
#include "SLText.h"

namespace VirtualRoom
{
	//-----------------------------------------------------------------------------
	//! tic tac toe game
	/*!      
	tic tac toe game based on the basis board game
	*/
	//-----------------------------------------------------------------------------
	class TicTacToeGameScene : public BoardGameScene 
	{
		public:
			TicTacToeGameScene(SkeletonData* defaultSkeletonData,  SLint localPlayerIndex, SLint maxPlayers, SLMat4f* playerPositions, VR_MODEL* models);
			~TicTacToeGameScene();

			void updateFromWorldState(RakNet::BitStream* bsIn, SkeletonData* defaultData);

		protected:
			virtual void resetSpecificGame() { };

		private:
			SLMaterial*		_matBoxes;	//!< box material
	};
}
//-----------------------------------------------------------------------------
#endif