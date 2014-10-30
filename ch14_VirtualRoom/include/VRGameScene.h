//#############################################################################
//  File:      VRGameScene.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VRGAMESCENE_H
#define VRGAMESCENE_H

#include <stdafx.h>
#include <VRWorldState.h>
#include "SLPerson.h"

namespace VirtualRoom {

	//-----------------------------------------------------------------------------
	//! base game scene
	/*!      
	includes all base functions of a virtual room game scene
	*/
	//-----------------------------------------------------------------------------
	class GameScene  {

		public:
			GameScene									(SkeletonData* defaultSkeletonData,  SLint localPlayerIndex, SLint maxPlayers, SLMat4f* playerPositions, VR_MODEL* models);
			virtual ~GameScene							();
			virtual void updateFromWorldState			(RakNet::BitStream* bsIn, SkeletonData* defaultData);
			void rebuildPersons							(SLbool* hasSkeletonData, SkeletonData* playerData, SkeletonData* defaultData);
			void rebuildLocalFigure						(SkeletonData* data);
			void getLocalPersonHandPos					(SLVec3f& leftHand, SLVec3f& rightHand);
			SLNode* getRootNode						() { return this->_rootNode; }
			void activateCam							(SLint playerIndex);

		protected:
			SLNode*		_rootNode;			//!< game root node
			SLPerson**	_persons;			//!< players
			SLint		_localPlayerIndex;	//!< index of the local player
			SLint		_maxPlayers;		//!< maximum players of the game

		private:
			SLMaterial* _matLocalPerson;	//!< material local person
			SLMaterial* _matExternalPerson;	//!< material external persons
	};
}
//-----------------------------------------------------------------------------
#endif