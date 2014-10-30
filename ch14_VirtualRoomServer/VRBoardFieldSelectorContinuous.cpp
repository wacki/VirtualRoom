//#############################################################################
//  File:      VRBoardFieldSelectorContinuous.cpp
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include "VRBoardFieldSelectorContinuous.h"

using namespace VirtualRoom::Game;

//-----------------------------------------------------------------------------
/*! 
until the field is full selected the selection color is becomes more intense
returns true if the field is full selected
*/
SLbool BoardFieldSelectorContinuous::select(BoardField* boardField, SLVec4f selectionColor)
{
	if (boardField == NULL)
	{
		this->reset();
	}
	else
	{
		if (_selectedField == NULL || _selectedField != boardField)
		{
			if (_selectedField != NULL)
				_selectedField->unselect();

			_actStep = 0;
			_selectedField = boardField;
			_selectedField->select(_selectedField->_initColor);
			_deltaVec = (selectionColor - _selectedField->_initColor) / NUMBER_OF_STEPS;
		}
		else
		{
			_actStep++;
			if (_actStep < NUMBER_OF_STEPS)
			{
				_selectedField->_color += _deltaVec;
			}
			else
			{
				this->reset();
				return true;
			}
		}
	}

	return false;
}
//-----------------------------------------------------------------------------
/*! 
resets the selector
*/
void BoardFieldSelectorContinuous::reset()
{
	if (_selectedField != NULL)
		_selectedField->unselect();

	_selectedField = NULL;
	_actStep = 0;
}
//-----------------------------------------------------------------------------