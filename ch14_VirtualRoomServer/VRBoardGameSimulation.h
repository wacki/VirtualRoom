//#############################################################################
//  File:      VRBoardGameSimulation.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VRBOARDGAMESIMULATION_H
#define VRBOARDGAMESIMULATION_H

namespace VirtualRoom
{
	namespace Game
	{
		//-----------------------------------------------------------------------------
		//! board game simulation
		/*!      
		simulates a board game
		*/
		//-----------------------------------------------------------------------------
		class BoardGameSimulation
		{
		public:
			BoardGameSimulation						() : _isFinished(false) { };
			virtual ~BoardGameSimulation			() { };

			virtual void doSimulationStep			() = 0;
			SLbool isSimulationFinished				() { return this->_isFinished; }

		protected:
			SLbool _isFinished;	//!< if the simulation is finished
		};
	}
}
//-----------------------------------------------------------------------------
#endif