//#############################################################################
//  File:      VRBoardFieldSelectorConfirm.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VRBOARDFIELDSELECTORCONFIRM_H
#define VRBOARDFIELDSELECTORCONFIRM_H

#include "VRBoardFieldSelector.h"

namespace VirtualRoom
{
	namespace Game
	{
		//-----------------------------------------------------------------------------
		//! confirm selector
		/*!      
		defines the select and confirm selection pattern
		*/
		//-----------------------------------------------------------------------------
		class BoardFieldSelectorConfirm : public BoardFieldSelector
		{
		public:
			BoardFieldSelectorConfirm			(): _selectedField(NULL), _isAnyFieldHit(false), BoardFieldSelector() { };
			
			SLbool select						(BoardField* boardField, SLVec4f selectionColor);
			void reset							();

		private:
			BoardField* _selectedField;	//!< the selected field
			SLbool		_isAnyFieldHit;	//!< is a field hitted

		};
	}
}
//-----------------------------------------------------------------------------
#endif