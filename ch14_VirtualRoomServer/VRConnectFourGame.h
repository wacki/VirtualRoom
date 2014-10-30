//#############################################################################
//  File:      VRConnectFourGame.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VRCONNECTFOURGAME_H
#define VRCONNECTFOURGAME_H

#include "VRConnectFourConstants.h"
#include "VRBoardGame.h"

namespace VirtualRoom
{
	namespace Game
	{
		//-----------------------------------------------------------------------------
		//! connect four game
		/*!      
		extends the basic board game for the connect four game
		*/
		//-----------------------------------------------------------------------------
		class ConnectFourGame : public BoardGame
		{
		public:
			ConnectFourGame						();
			~ConnectFourGame					();
			
			virtual BoardGameSimulation* getMoveSimulation	(BoardPlayField* boardPlayField, int indexLastSelectedX, int indexLastSelectedY, int indexLastSelectedZ);

			void updateGameState							(int indexLastSelectedX, int indexLastSelectedY, int indexLastSelectedZ);
			WorldState* getBoardState						(SLTimer &timer, SLint maxPlayers);
			BoardPlayField* checkHit						(SLVec3f& effectivePosLeftHand, SLVec3f& effectivePosRightHand, int& indexLastSelectedX, int& indexLastSelectedY, int& indexLastSelectedZ);

		private:
			SLfloat* _cylinderMinX;		//!< cylinder min x
			SLfloat* _cylinderMaxX;		//!< cylinder max x 
			SLfloat* _cylinderMinY;		//!< cylinder min y 
			SLfloat* _cylinderMaxY;		//!< cylinder max y 
			SLfloat* _cylinderMinZ;		//!< cylinder min z 
			SLfloat* _cylinderMaxZ;		//!< cylinder max z 

			SLbool checkHitWithCylinder			(SLuint cylinderIndex, SLVec3f armPosition);
		};
	}
}
//-----------------------------------------------------------------------------
#endif