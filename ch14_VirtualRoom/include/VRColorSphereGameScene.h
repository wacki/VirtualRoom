//#############################################################################
//  File:      VRColorSphereGameScene.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VRCOLORSPHEREGAMESPHERE_H
#define VRCOLORSPHEREGAMESPHERE_H

#include "VRGameScene.h"
#include "SLSphere.h"
#include <VRColorSphereWorldState.h>

namespace VirtualRoom {
	
	//-----------------------------------------------------------------------------
	//! color sphere game
	/*!      
	a minimal game with a colored sphere
	*/
	//-----------------------------------------------------------------------------
	class ColorSphereGameScene : public GameScene  {

		public:
			ColorSphereGameScene(SkeletonData* defaultSkeletonData,  SLint localPlayerIndex, SLint maxPlayers, SLMat4f* playerPositions, VR_MODEL* models);
			~ColorSphereGameScene();

			void updateFromWorldState(RakNet::BitStream* bsIn, SkeletonData* defaultData);

		protected:
			SLSphere*	_sphere;	//!< colored sphere

		private:
			SLMaterial*	_matSphere;	//!< sphere material
	};
}
//-----------------------------------------------------------------------------
#endif