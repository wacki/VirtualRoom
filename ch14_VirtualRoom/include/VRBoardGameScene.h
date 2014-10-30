//#############################################################################
//  File:      VRBoardGameScene.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VRBOARDGAMESCENE_H
#define VRBOARDGAMESCENE_H

#include "VRGameScene.h"
#include "VRBoardGameWorldState.h"
#include "VRBoardGameConstants.h"
#include "SLBox.h"
#include "SLSphere.h"
#include "SLText.h"

namespace VirtualRoom
{
	//-----------------------------------------------------------------------------
	//! board game base class
	/*!      
	includes all standard board game functions
	*/
	//-----------------------------------------------------------------------------
	class BoardGameScene : public GameScene 
	{
		
	public:
		BoardGameScene							(SkeletonData* defaultSkeletonData,  SLint localPlayerIndex, SLint maxPlayers, SLMat4f* playerPositions, VR_MODEL* models);
		~BoardGameScene							();
		virtual void updateFromWorldState		(RakNet::BitStream* bsIn, SkeletonData* defaultData) = 0;
		
	protected:
		VR_BOARDGAME_STATE _gameState;			//!< current game state

		void changeGameState					(SLint activePlayerId, SLstring endText, SLfloat heightOfText);
		void updateBoardGameFromWroldState		(BoardGameWorldState* message, SkeletonData* defaultData);

		virtual void resetSpecificGame			() = 0;

	private:
		SLMaterial**	_matResetBoxesForPlayer;	//!< material of the reset boxes
		SLGLTexture*	_textResetBoxes;			//!< texture of the reset boxes
		SLText**		_textesForPlayer;			//!< player text
		SLNode**		_resetBoxesForPlayer;		//!< reset box groups
		
	};
}
//-----------------------------------------------------------------------------
#endif