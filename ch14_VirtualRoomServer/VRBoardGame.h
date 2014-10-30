//#############################################################################
//  File:      VRBoardGame.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VRBOARDGAME_H
#define VRBOARDGAME_H

#include <stdafx.h>					// precompiled headers with math stuff
#include "VRBoardGameConstants.h"
#include "VRWorldState.h"
#include "VRBoardPlayField.h"
#include "VRPlayer.h"
#include "VRBoardGameSimulation.h"
#include "VRBoardFieldSelectorContinuous.h"

namespace VirtualRoom
{
	namespace Game
	{
		//-----------------------------------------------------------------------------
		//! board game base class
		/*!      
		inclueds all needed functionalities for a board game
		*/
		//-----------------------------------------------------------------------------
		class BoardGame
		{
		public:
			VR_GAME_TYPE						_gameType;							//!< game type
			SLuint								_numberOfPlayers;					//!< number of players in the game

			BoardGame							(SLuint numberOfPlayers, VR_GAME_TYPE gameType, BoardFieldSelector* playFieldSelector, SLuint numberOfLevels, SLuint numberOfColumns, SLuint numberOfRows);
			virtual ~BoardGame					();

			void moveToNextPlayer				(int indexLastSelectedX, int indexLastSelectedY, int indexLastSelectedZ);
			SLint getActPlayerId				();
			SLbool isRuning						();
			SLbool hasActiveSimulation			();
			void play							(SLVec3f& effectivePosLeftHand, SLVec3f& effectivePosRightHand);
			void handleReset					(int playerIndex, SLVec3f& effectivePosLeftHand, SLVec3f& effectivePosRightHand);
			void setPersonMatrices				(SLMat4f* personMatrices);
			void doSimulationStep				();

			virtual void updateGameState		(int indexLastSelectedX, int indexLastSelectedY, int indexLastSelectedZ) = 0;
			virtual WorldState* getBoardState	(SLTimer &timer, SLint maxPlayers) = 0;
			virtual BoardPlayField* checkHit	(SLVec3f& effectivePosLeftHand, SLVec3f& effectivePosRightHand, int& indexLastSelectedX, int& indexLastSelectedY, int& indexLastSelectedZ) = 0;

		protected:
			VR_BOARDGAME_STATE					_gameState;							//!< state of the game
			SLint								_actPlayerIndex;					//!< index of the active player
			Player*								_players;							//!< players of the game
			BoardPlayField***					_boardPlayFields;					//!< fields of the game
			BoardField*							_resetBoxes;						//!< game reset boxes
			SLuint								_numberOfBoardPlayFieldRows;		//!< board game row count
			SLuint								_numberOfBoardPlayFieldColumns;		//!< board game column count
			SLuint								_numberOfBoardPlayFieldLevels;		//!< board game level count

			void finisheGame					(VR_BOARDGAME_STATE state);

		private:
			BoardFieldSelector*					_playFieldSelector;					//!< selector for the fields
			BoardFieldSelectorContinuous*		_resetBoxesSelectors;				//!< selector for the reset boxes
			BoardGameSimulation*				_activeSimulation;					//!< active game simulations
			SLMat4f*							_personMatrices;					//!< player matrices

			virtual BoardGameSimulation* getMoveSimulation (BoardPlayField* boardPlayField, int indexLastSelectedX, int indexLastSelectedY, int indexLastSelectedZ) { return NULL; };

			void reset							();
			void unselectAllFields				();
		};
	}
}
//-----------------------------------------------------------------------------
#endif