//#############################################################################
//  File:      VRConnectFourGameScene.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VRCONNECTFOURGAMESCENE_H
#define VRCONNECTFOURGAMESCENE_H

#include "VRBoardGameScene.h"
#include "VRConnectFourWorldState.h"
#include "VRConnectFourConstants.h"
#include "SLBox.h"
#include "SLSphere.h"
#include "SLText.h"

namespace VirtualRoom
{
	//-----------------------------------------------------------------------------
	//! connect four game
	/*!      
	connect four game based on the basis board game
	*/
	//-----------------------------------------------------------------------------
	class ConnectFourGameScene : public BoardGameScene 
	{
		public:
			ConnectFourGameScene(SkeletonData* defaultSkeletonData,  SLint localPlayerIndex, SLint maxPlayers, SLMat4f* playerPositions, VR_MODEL* models);
			~ConnectFourGameScene();

			void updateFromWorldState(RakNet::BitStream* bsIn, SkeletonData* defaultData);

		private:
			SLMaterial**	_matCylinders;			//!< material cylinders
			SLMaterial* 	_matGround;				//!< material ground plate
			SLMaterial*	    _matFeet;				//!< material feet of plate
			SLSphere**		_spheres;				//!< game spheres
			SLNode**		_sphereNodes;				//!< game spheres
			SLMaterial**	_matSpheres;			//!< material of the game spheres
			SLVec3f*		_actualSpherePositions;	//!< game sphere position
			SLint			_numberOfSpheres;		//!< current number of spheres

			void resetSpecificGame();
	};
}
//-----------------------------------------------------------------------------
#endif