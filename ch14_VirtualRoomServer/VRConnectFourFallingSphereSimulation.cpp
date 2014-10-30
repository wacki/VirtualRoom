//#############################################################################
//  File:      VRConnectFourFallingSphereSimulation.cpp
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include "VRConnectFourFallingSphereSimulation.h"

using namespace VirtualRoom::Game;

//-----------------------------------------------------------------------------
ConnectFourFallingSphereSimulation::ConnectFourFallingSphereSimulation(BoardPlayField* boardPlayField, SLint zIndex) : BoardGameSimulation() , _boardPlayField(boardPlayField), _actualStep(0)
{
	this->_boardPlayField->_actualPosition = this->_boardPlayField->_simulationStartPosition;

	this->_effectiveNumberOfSteps = NUMBER_OF_STEPS_PER_SPHERE * (CONNECTFOUR_NUMBER_OF_ROWS_COLUMNS_LEVELS - zIndex);

	this->_deltaVector = (this->_boardPlayField->_simulationEndPosition - this->_boardPlayField->_simulationStartPosition) / (this->_effectiveNumberOfSteps);
}
//-----------------------------------------------------------------------------
/*! 
simulates the sphere
*/
void ConnectFourFallingSphereSimulation::doSimulationStep ()
{
	if(this->_actualStep < this->_effectiveNumberOfSteps)
	{
		this->_boardPlayField->_actualPosition += this->_deltaVector;
		this->_actualStep++;
	}
	else
	{
		this->_boardPlayField->_actualPosition = this->_boardPlayField->_simulationEndPosition;
		this->_isFinished  = true;
	}
}
//-----------------------------------------------------------------------------