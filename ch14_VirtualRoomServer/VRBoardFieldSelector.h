//#############################################################################
//  File:      VRBoardFieldSelector.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VRBOARDFIELDSELECTOR_H
#define VRBOARDFIELDSELECTOR_H

#include <stdafx.h>
#include "VRBoardField.h"

namespace VirtualRoom
{
	namespace Game
	{
		//-----------------------------------------------------------------------------
		//! selector base class
		/*!      
		the selector defines how a field can be selected
		*/
		//-----------------------------------------------------------------------------
		class BoardFieldSelector
		{
		public:
			BoardFieldSelector			() { };
			virtual SLbool select		(BoardField* boardField, SLVec4f selectionColor) = 0;
			virtual void reset			() = 0;
		};
	}
}
//-----------------------------------------------------------------------------
#endif