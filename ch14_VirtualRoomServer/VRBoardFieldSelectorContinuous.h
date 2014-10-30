//#############################################################################
//  File:      VRBoardFieldSelectorContinuous.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VRBOARDFIELDSELECTORCONTINUOUS_H
#define VRBOARDFIELDSELECTORCONTINUOUS_H

#include "VRBoardFieldSelector.h"

namespace VirtualRoom
{
	namespace Game
	{
		//-----------------------------------------------------------------------------
		//! continuous selector
		/*!      
		defines the continuous selection pattern
		*/
		//-----------------------------------------------------------------------------
		class BoardFieldSelectorContinuous : public BoardFieldSelector
		{
		public:
			BoardFieldSelectorContinuous		(): _selectedField(NULL), _actStep(0), BoardFieldSelector() { };
			
			SLbool select						(BoardField* boardField, SLVec4f selectionColor);
			void reset							();
		
		private:
			SLint					_actStep;				//!< current selection step
			SLVec4f					_deltaVec;				//!< selection delta
			BoardField*				_selectedField;			//!< current selected field
			static const SLuint		NUMBER_OF_STEPS = 100;	//!< number of selection steps
		};
	}
}
//-----------------------------------------------------------------------------
#endif