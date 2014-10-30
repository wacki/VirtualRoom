//#############################################################################
//  File:      VRBoardPlayField.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VRBOARDPLAYFIELD_H
#define VRBOARDPLAYFIELD_H

#include "VRBoardField.h"
#include "VRPlayer.h"

namespace VirtualRoom
{
	namespace Game
	{
		//-----------------------------------------------------------------------------
		//! extended board field
		/*!      
		the board play field extends the boar field. It can simulate a move, can be
		owned from a player and can be part of a winner group
		*/
		//-----------------------------------------------------------------------------
		class BoardPlayField : public BoardField
		{
		public:
			BoardPlayField				() : _owner(NULL), _isWinner(false), BoardField() { };

			SLint getOwnerId			();
			void reset					();

			SLVec3f		_simulationStartPosition;	//!< simulation start
			SLVec3f		_simulationEndPosition;		//!< simulation end
			SLbool		_isWinner;					//!< is the field one of the winner fields
			Player*		_owner;						//!< owner of the field
		};
	}
}
//-----------------------------------------------------------------------------
#endif