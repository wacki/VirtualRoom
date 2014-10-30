//#############################################################################
//  File:      VRBoardField.cpp
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include "VRBoardField.h"

using namespace VirtualRoom::Game;

//-----------------------------------------------------------------------------
/*! 
checks if the given position is in the field
*/
SLbool BoardField::checkHit(SLVec3f armPosition)
{
	return (armPosition.x > (this->_actualPosition.x - HAND_SELECTION_RADIUS) && armPosition.x < (this->_actualPosition.x + this->_width + HAND_SELECTION_RADIUS) &&
		armPosition.y > (this->_actualPosition.y - HAND_SELECTION_RADIUS) && armPosition.y < (this->_actualPosition.y + this->_heigh + HAND_SELECTION_RADIUS) &&
		armPosition.z > (this->_actualPosition.z - HAND_SELECTION_RADIUS) && armPosition.z < (this->_actualPosition.z + this->_depth + HAND_SELECTION_RADIUS));
};
//-----------------------------------------------------------------------------
/*! 
set the color of the field and mark it as selected
*/
void BoardField::select(SLVec4f selectionColor)
{
	this->_selected = true;
	this->_color = selectionColor;
};
//-----------------------------------------------------------------------------
/*! 
set the initial color of the field an mark it as deselected
*/
void BoardField::unselect()
{
	this->_selected = false;
	this->_color = this->_initColor;
};
//-----------------------------------------------------------------------------
/*! 
resets the field
*/
void BoardField::reset()
{
	this->unselect();
}
//-----------------------------------------------------------------------------