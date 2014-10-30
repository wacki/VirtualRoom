//#############################################################################
//  File:      VRPlayer.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VRPLAYER_H
#define VRPLAYER_H

namespace VirtualRoom
{
	namespace Game
	{

		class BoardPlayField;				// Foreward declaration
		class BoardGameSimulation;			// Foreward declaration

		//-----------------------------------------------------------------------------
		//! player class
		/*!      
		the player class represents a player of a game
		*/
		//-----------------------------------------------------------------------------
		class Player
		{
		public:
			Player									() { };
			virtual ~Player							() { };

			void setId								(SLint id) { this->_id = id; };
			SLint getId								() { return this->_id; };
			SLstring getName						() { return this->_name; };
			void setName							(SLstring name) { this->_name = name; };
			SLVec4f getColor						() { return this->_color; };
			SLVec4f getSelectionColor				() { return this->_selectionColor; };
			void setColor							(SLVec4f color) { this->_color = color; };
			void setSelectionColor					(SLVec4f selectionColor) { this->_selectionColor = selectionColor; };

		protected:
			SLint					_id;				//!< player id
			SLVec4f					_color;				//!< player color
			SLVec4f					_selectionColor;	//!< selection color

		private:
			SLstring				_name;				//!< player name
		};
	}
}
//-----------------------------------------------------------------------------
#endif