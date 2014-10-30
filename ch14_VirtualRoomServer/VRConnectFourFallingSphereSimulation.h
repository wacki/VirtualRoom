//#############################################################################
//  File:      VRConnectFourFallingSphereSimulation.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VRCONNECTFOURFALLINGSPHERESIMULATION_H
#define VRCONNECTFOURFALLINGSPHERESIMULATION_H

#include <stdafx.h>					// precompiled headers with math stuff
#include "VRConnectFourConstants.h"
#include "VRBoardGameSimulation.h"
#include "VRBoardPlayField.h"

namespace VirtualRoom
{
	namespace Game
	{
		//-----------------------------------------------------------------------------
		//! simulation of a sphere in the connect four game
		/*!      
		manages the simulation of the sphere from the connect four game
		*/
		//-----------------------------------------------------------------------------
		class ConnectFourFallingSphereSimulation : public BoardGameSimulation
		{
		public:
			ConnectFourFallingSphereSimulation		(BoardPlayField* bardPlayField, SLint zIndex);

			void doSimulationStep					();

		private:
			BoardPlayField*		_boardPlayField;					//!< field of the simulated sphere
			SLVec3f				_deltaVector;						//!< simulation delta
			SLuint				_actualStep;						//!< current simulation step
			SLuint				_effectiveNumberOfSteps;			//!< effective number of steps
			static const SLuint NUMBER_OF_STEPS_PER_SPHERE = 15;	//!< maximal number of steps
		};
	}
}
//-----------------------------------------------------------------------------
#endif