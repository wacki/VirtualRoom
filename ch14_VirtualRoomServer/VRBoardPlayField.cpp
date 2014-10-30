//#############################################################################
//  File:      VRBoardPlayField.cpp
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include "VRBoardPlayField.h"

using namespace VirtualRoom::Game;

//-----------------------------------------------------------------------------
/*! 
return the if of the owner player
*/
SLint BoardPlayField::getOwnerId()
{
	if (this->_owner == NULL)
		return BOARDGAME_NO_OWNER_CODE;

	return this->_owner->getId();
}
//-----------------------------------------------------------------------------
/*! 
resets the field
*/
void BoardPlayField::reset()
{
	BoardField::reset();
	this->_owner = NULL;
	this->_isWinner = false;
}
//-----------------------------------------------------------------------------