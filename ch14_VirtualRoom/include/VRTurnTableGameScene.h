//#############################################################################
//  File:      VRTurnTableGameScene.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VRTURNTABLEGAMESCENE_H
#define VRTURNTABLEGAMESCENE_H

#include "VRGameScene.h"

namespace VirtualRoom {
	
	//-----------------------------------------------------------------------------
	//! turn table game
	/*!      
	minimal turn table game
	*/
	//-----------------------------------------------------------------------------
	class TurnTableGameScene : public GameScene  {

	public:
		TurnTableGameScene(SkeletonData* defaultSkeletonData,  SLint localPlayerIndex, SLint maxPlayers, SLMat4f* playerPositions, VR_MODEL* models);
		~TurnTableGameScene();

		void updateFromWorldState(RakNet::BitStream* bsIn, SkeletonData* defaultData);

	private:
		SLMaterial* _matTable;	//!< table material
		SLNode*	_table;		//!< table group
	};
}
//-----------------------------------------------------------------------------
#endif