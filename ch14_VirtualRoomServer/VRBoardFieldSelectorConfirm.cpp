//#############################################################################
//  File:      VRBoardPlayFieldSelectorConfirm.cpp
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include "VRBoardFieldSelectorConfirm.h"

using namespace VirtualRoom::Game;

//-----------------------------------------------------------------------------
/*! 
returns true(confirm) if the selected field is already selected
*/
SLbool BoardFieldSelectorConfirm::select(BoardField* boardField, SLVec4f selectionColor)
{
	if (boardField == NULL)
	{
		_isAnyFieldHit = false;
	}
	else
	{
		if (_selectedField == NULL)
		{
			_selectedField = boardField;
			_selectedField->select(selectionColor);
			_isAnyFieldHit = true;
		}
		else
		{
			if (!_isAnyFieldHit && _selectedField == boardField)
			{
				this->reset();
				return true;
			}
			else if (_selectedField != boardField)
			{
				_selectedField->unselect();
				_selectedField = boardField;
				_selectedField->select(selectionColor);
				_isAnyFieldHit = true;
			}
		}
	}

	return false;
}
//-----------------------------------------------------------------------------
/*! 
resets the selector
*/
void BoardFieldSelectorConfirm::reset()
{
	if (_selectedField != NULL)
		_selectedField->unselect();

	_isAnyFieldHit = false;
	_selectedField = NULL;
}
//-----------------------------------------------------------------------------