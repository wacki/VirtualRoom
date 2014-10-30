//#############################################################################
//  File:      VRTicTacToeConstants.h
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#ifndef VRTICTACTOECONSTANTS_H
#define VRTICTACTOECONSTANTS_H

#include "VRBoardGameConstants.h"

#define TICTACTOE_BOARDGAME_FIELD_WIDTH				0.25f
#define TICTACTOE_BOARDGAME_FIELD_HEIGH				0.25f
#define TICTACTOE_BOARDGAME_FIELD_DEPTH				0.10f
#define TICTACTOE_BOARDGAME_FIELD_BOARDER			0.10f

#define TICTACTOE_BOARDGAME_Y_OFFSET				1.00f

#define TICTACTOE_NUMBER_OF_ROWS_COLUMNS			3
#define TICTACTOE_NUMBER_OF_FIELDS					(TICTACTOE_NUMBER_OF_ROWS_COLUMNS * TICTACTOE_NUMBER_OF_ROWS_COLUMNS)

#define TICTACTOE_PLAYER0_COLOR_NAME				"Green"
#define TICTACTOE_PLAYER1_COLOR_NAME				"Red"
#define TICTACTOE_PLAYER0_COLOR_VALUE				SLCol4f(0.0f, 1.0f, 0.0f, BOX_ALPHA_VALUE)
#define TICTACTOE_PLAYER1_COLOR_VALUE				SLCol4f(1.0f, 0.0f, 0.0f, BOX_ALPHA_VALUE)

#endif