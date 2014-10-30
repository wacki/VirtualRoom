//#############################################################################
//  File:      VRBoardField.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VRBOARDFIELD_H
#define VRBOARDFIELD_H

#include <stdafx.h>
#include "VRBoardGameConstants.h"

namespace VirtualRoom
{
	namespace Game
	{
		//-----------------------------------------------------------------------------
		//! the field of a board game
		/*!      
		represent a single field of a board game. Includes all important informations
		about the field
		*/
		//-----------------------------------------------------------------------------
		class BoardField
		{
		public:
			BoardField					() : _selected(false), _color(SLVec4f(1.0f, 1.0f, 1.0f, BOX_ALPHA_VALUE)), _initColor(SLVec4f(1.0f, 1.0f, 1.0f, BOX_ALPHA_VALUE)) { };

			SLbool checkHit				(SLVec3f armPosition);
			void select					(SLVec4f selectionColor);
			void unselect				();
			void reset					();

			SLVec3f		_actualPosition;	//!< field position
			SLfloat		_heigh;				//!< field height
			SLfloat		_width;				//!< field width
			SLfloat		_depth;				//!< field depth
			SLCol4f		_color;				//!< current field color
			SLCol4f		_initColor;			//!< initial field color
			SLbool		_selected;			//!< is the field selected
		};
	}
}
//-----------------------------------------------------------------------------
#endif