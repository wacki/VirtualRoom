//#############################################################################
//  File:      VRServerStarter.cpp
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include "VRServer.h"
#include "VRColorSphereServer.h"
#include "VRBoardGameServer.h"
#include "VRTicTacToeGame.h"
#include "VRConnectFourGame.h"
#include "VRTurnTableServer.h"
#include <windows.h>

VirtualRoom::Server* server;

#define GAME_TYPE_ARGUMENT_INDEX 0
#define NUMBR_PLAYERS_ARGUMENT_INDEX 1

//-----------------------------------------------------------------------------
/*! 
handles the controll events
*/
BOOL CtrlHandler( DWORD fdwCtrlType ) 
{ 
	switch( fdwCtrlType ) 
	{ 
		// Handle the CTRL-C signal. 
	case CTRL_C_EVENT: 
		server->stop();
		return( TRUE );

		// CTRL-CLOSE: confirm that the user wants to exit. 
	case CTRL_CLOSE_EVENT: 
		server->stop();
		return( TRUE ); 

	case CTRL_BREAK_EVENT: 
		server->stop();
		return TRUE; 

	case CTRL_LOGOFF_EVENT: 
		server->stop();
		return TRUE; 

	case CTRL_SHUTDOWN_EVENT: 
		server->stop();
		return TRUE; 

	default: 
		return FALSE;
	} 
}
//-----------------------------------------------------------------------------
/*! 
initates the server and starts a game
*/
int main(int argc, char *argv[])
{  
	// set command line arguments
	vector<string>* commandLineArguments = new vector<string>;
	for(int i = 1; i < argc; i++)
		commandLineArguments->push_back(argv[i]);

	VirtualRoom::VR_GAME_TYPE gameType = VirtualRoom::VR_GAME_TYPE::VR_GAME_TYPE_DEFAULT;
	SLint numberOfPlayers = 2;

	// set game type
	if(commandLineArguments->size() > GAME_TYPE_ARGUMENT_INDEX)
		gameType = (VirtualRoom::VR_GAME_TYPE)atoi(commandLineArguments->at(GAME_TYPE_ARGUMENT_INDEX).c_str());

	// set number of players
	if(commandLineArguments->size() > NUMBR_PLAYERS_ARGUMENT_INDEX)
		numberOfPlayers = (SLint)atoi(commandLineArguments->at(NUMBR_PLAYERS_ARGUMENT_INDEX).c_str());

	switch(gameType)
	{
	case VirtualRoom::VR_GAME_TYPE::VR_GAME_TYPE_COLOR_SPHERE:
		server = new VirtualRoom::ColorSphereServer(numberOfPlayers);
		break;
	case VirtualRoom::VR_GAME_TYPE::VR_GAME_TYPE_TIC_TAC_TOE:
		server = new VirtualRoom::BoardGameServer(new VirtualRoom::Game::TicTacToeGame);
		break;
	case VirtualRoom::VR_GAME_TYPE::VR_GAME_TYPE_CONNECT_FOUR:
		server = new VirtualRoom::BoardGameServer(new VirtualRoom::Game::ConnectFourGame);
		break;
	case VirtualRoom::VR_GAME_TYPE::VR_GAME_TYPE_TURN_TABLE:
		server = new VirtualRoom::TurnTableServer(numberOfPlayers);
		break;
	default:
		server = new VirtualRoom::Server(numberOfPlayers, VirtualRoom::VR_GAME_TYPE_DEFAULT);
	}

	// Set Console Control Handler for stopping the actual server by CTRL-C
	bool setConsolControlSuccessfully = SetConsoleCtrlHandler((PHANDLER_ROUTINE) CtrlHandler, TRUE);
	if(setConsolControlSuccessfully)
	{
		server->connect();
		server->run();
	} 
	else 
	{
		printf( "\nERROR: Could not set control handler");
	}

	delete server;

	// delete command line argument
	commandLineArguments->clear();
	delete commandLineArguments;

	if (!setConsolControlSuccessfully)
		return 1;

	return 0;

}
//-----------------------------------------------------------------------------