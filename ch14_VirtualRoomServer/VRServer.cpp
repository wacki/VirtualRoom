//#############################################################################
//  File:      VRServer.cpp
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include "VRServer.h"

using namespace VirtualRoom;

//-----------------------------------------------------------------------------
Server::Server(SLint maxPlayers, VR_GAME_TYPE gameType)
	: _gameType(gameType), _maxPlayers(maxPlayers)
{
	_maxClients = maxPlayers + MAX_OBSERVERS;
	_running	= false;

	// Init Starting positions and send server data to client or not
	_personMatrices			= new SLMat4f[_maxPlayers];
	_personModels			= new VR_MODEL[_maxPlayers];
	_skeletonDatas			= new SkeletonData[_maxPlayers];
	_leftHandPos			= new SLVec3f[_maxPlayers];
	_rightHandPos			= new SLVec3f[_maxPlayers];
	_hasSkeletonData		= new SLbool[_maxPlayers];
	_playerStates			= new PlayerState;
	_sendDataToClientIndex	= new VR_CLIENT_TYPE[_maxClients];
	_requestPingAnswer		= new SLbool[_maxClients];

	// player default values
	float angle = 360.0f / _maxPlayers;
	_playerStates->activePlayerCount	= 0;
	_playerStates->nextFreeSlot			= 0;
	_playerStates->clientIndex			= new SLint[_maxPlayers];

	for(int i = 0; i < _maxClients; i++)
	{
		// init player stuff
		if(i < _maxPlayers){
			// start position
			_personMatrices[i].identity();
			_personMatrices[i].rotate(angle*i, 0.0f, 1.0f, 0.0f);
			_personMatrices[i].translate(0.0f, 0.0f, DISTANCE_TO_OBJECT);

			// set models based on index
			if(i%2 == 0)	_personModels[i] = VR_HL2_CIV_MALE;
			else			_personModels[i] = VR_HL2_CIV_FEMALE;

			// network client
			_playerStates->clientIndex[i] = -1;

			// skeleton data
			_hasSkeletonData[i] = false;
		}
		
		// client state
		_sendDataToClientIndex[i] = VR_CLIENT_NOT_CONNECTED;
		_requestPingAnswer[i] = false;
	}
}
//-----------------------------------------------------------------------------
Server::~Server()
{
	// Cleanup
	delete[] _personMatrices;
	delete[] _personModels;
	delete[] _sendDataToClientIndex;
	delete[] _requestPingAnswer;
	delete[] _skeletonDatas;
	delete[] _leftHandPos;
	delete[] _rightHandPos;
	delete[] _hasSkeletonData;
	delete[] _playerStates->clientIndex;
	delete	 _playerStates;

	// Close RakNet peer properly
	RakNet::RakPeerInterface::DestroyInstance(_peer);
}
//-----------------------------------------------------------------------------
/*! 
initiates the server socket
*/
void Server::connect()
{
	SL_LOG("\n----------------------------------------------------\n");
	SL_LOG("Starting the Virtual Room Server ...");

	// Get RakNet instance
	_peer = RakNet::RakPeerInterface::GetInstance();

	// Describe socket by its server port and address
	RakNet::SocketDescriptor sd(SERVER_PORT,0);
	_peer->SetMaximumIncomingConnections(_maxClients);
	_peer->Startup(_maxClients, &sd, 1);

	SL_LOG("\tstartet!\n----------------------------------------------------\n");

	_running = true;
}
//-----------------------------------------------------------------------------
/*! 
server main loop
*/
void Server::run()
{
	if(_peer == NULL)
		return;

	// Ref to actual package
	RakNet::Packet* packet;
	
	double startTick;
	double endTick;
	double dif;

	// Main Loop
	while (_running)
	{
		startTick = _timer.getElapsedTimeInMilliSec();

		for (packet = _peer->Receive(); packet; _peer->DeallocatePacket(packet), packet = _peer->Receive())
		{
			this->handlePacket(packet);
		}

		simulate();

		// Send pings
		sendRequestedPings();

		// Sleep for not making the process use all the cpu
		endTick = _timer.getElapsedTimeInMilliSec();
		dif = endTick - startTick;
		if (dif < REFRESH_RATE)
		{
			Sleep(REFRESH_RATE - dif);
		}

		// Send data to all clients receiving 
		this->sendDataToAllClients();
	}

	SL_LOG("\n----------------------------------------------------\n");
	SL_LOG("Stopping the server ...");

	RakNet::SystemAddress* systems = new RakNet::SystemAddress[_maxClients];
	unsigned short numConnections = _maxClients;
	_peer->GetConnectionList(systems, &numConnections);
	for (int i=0; i < numConnections; i++)
	{
		_peer->CloseConnection(systems[i], true);
	}
	delete[] systems;

	unsigned short attempts = 0;
	while(_peer->NumberOfConnections() > 0 && attempts < 100)
	{
		// Wait a little for letting raknet finisch closing the connections
		Sleep(100);
		attempts++;
	}

	if (_peer->NumberOfConnections() > 0)
		SL_LOG("\n\nSome clients did not disconnect within 10 secs!\n");

	SL_LOG("\tstopped!\n----------------------------------------------------\n");
}
//-----------------------------------------------------------------------------
/*! 
stops the server
*/
void Server::stop()
{
	if (_peer != NULL && _running)
		_running = false;
}
//-----------------------------------------------------------------------------
/*! 
handles the packets from the clients
*/
void Server::handlePacket(RakNet::Packet* packet)
{
	unsigned short index = _peer->GetIndexFromSystemAddress(packet->systemAddress);
	unsigned char packetIdentifier = Network::GetPacketIdentifier(packet);

	switch (packetIdentifier)
	{
	case ID_NEW_INCOMING_CONNECTION:
		SL_LOG("\n----------------------------------------------------\n");
		SL_LOG("A connection is incoming: %s\n", packet->systemAddress.ToString(true, ':'));
		SL_LOG("%u clients connected at the moment.\n", _peer->NumberOfConnections());
		SL_LOG("----------------------------------------------------\n");
		break;
	case ID_DISCONNECTION_NOTIFICATION:
		this->disconnectClient(index);
		break;
	case ID_CONNECTION_LOST:
		this->disconnectClient(index);
		break;
	case Network::ID_CONNECTING_QUESTION:
		this->requestStartAddress(index, packet);
		break;
	case Network::ID_DATA_CLIENT:
		this->receiveDataFromClient(index, packet);
		break;
	case Network::ID_PING_PACKET:
		{
			SL_LOG("\n%u - requesting ping answer\n", index);
			this->_requestPingAnswer[index] = true;
		}
		break;
	default:
		SL_LOG("\nMessage with unhandelt identifier %i has arrived.\n\n", packetIdentifier);
		break;
	}
}
//-----------------------------------------------------------------------------
/*! 
disconnects the given client
*/
void Server::disconnectClient(unsigned short index)
{
	SL_LOG("\n----------------------------------------------------\n");
	SL_LOG("\nClient with index '%u' has disconnected.\n", index);

	// special disconnection actions for player clients
	if(_sendDataToClientIndex[index] == VR_CLIENT_TYPE_PLAYER)
	{
		_playerStates->activePlayerCount--;
		// clear disconnected player
		for (int i=_maxPlayers-1; i >= 0; i--)
		{
			// clear player to client index
			if(_playerStates->clientIndex[i] == index)
				_playerStates->clientIndex[i] = -1;

			// find first free slot
			if(_playerStates->clientIndex[i] == -1)
				_playerStates->nextFreeSlot = i;
		}		
	}

	_sendDataToClientIndex[index] = VR_CLIENT_NOT_CONNECTED;
	_requestPingAnswer[index] = false;
	_hasSkeletonData[index] = false;
	
	SL_LOG("%u clients connected at the moment.\n\n", _peer->NumberOfConnections());
	SL_LOG("----------------------------------------------------\n");
}
//-----------------------------------------------------------------------------
/*! 
handle start request packet
*/
void Server::requestStartAddress(unsigned short index, RakNet::Packet* packet)
{
	Network::ConnectingQuestion* request = Network::GetStructFromDataMessage<Network::ConnectingQuestion>(packet);

	if(request->_clientType == VR_CLIENT_TYPE_PLAYER)
	{
		if(_playerStates->activePlayerCount < _maxPlayers)
		{
			SL_LOG("\n%u - requesting start position\n", index);
			// refresh player states
			Network::ConnectingAnswer message(_timer.getElapsedTimeInMilliSec(), _maxPlayers, _playerStates->nextFreeSlot, _gameType);
			_playerStates->activePlayerCount++;
			_playerStates->clientIndex[_playerStates->nextFreeSlot] = index;

			if(_playerStates->activePlayerCount != _maxPlayers)
			{
				// find new nextFreeSlot
				for (int i=0; i < _maxPlayers; i++)
				{
					if(_playerStates->clientIndex[i] == -1)
					{
						_playerStates->nextFreeSlot = i;
						break;
					}
				}
									
			}

			// set starting positions
			message._initMat = _personMatrices;
			// set models per person
			message._initModels = _personModels;
			
			SL_LOG("%u - sending start position at timestamp %f\n\n", index, message.getTimeStamp());
			
			Network::SendStructMessage(_peer, packet->systemAddress, message, HIGH_PRIORITY, RELIABLE);

			// From now on send also the server data to the client
			_sendDataToClientIndex[index] = VR_CLIENT_TYPE_PLAYER;
		}else
		{
			SL_LOG("\nclient connection closed: maximum players already reached\n");
			_peer->CloseConnection(packet->systemAddress, true);
		}
	}else {
		if(_peer->NumberOfConnections() - _playerStates->activePlayerCount < MAX_OBSERVERS){
			Network::ConnectingAnswer message(_timer.getElapsedTimeInMilliSec(),  _maxPlayers, -1, _gameType);

			// set starting positions
			message._initMat = _personMatrices;
			// set models per person
			message._initModels = _personModels;

			SL_LOG("%u - sending start position at timestamp %f\n\n", index, message.getTimeStamp());
			
			Network::SendStructMessage(_peer, packet->systemAddress, message, HIGH_PRIORITY, RELIABLE);

			// From now on send also the server data to the client
			_sendDataToClientIndex[index] = VR_CLIENT_TYPE_OBSERVER;
		}else{
			SL_LOG("\nclient connection closed: maximum observers already reached\n");
			_peer->CloseConnection(packet->systemAddress, true);
		}
	}
	delete request;
}
//-----------------------------------------------------------------------------
/*! 
handles data package from the clients
*/
void Server::receiveDataFromClient(unsigned short index, RakNet::Packet* packet)
{
	Network::ClientData* message = Network::GetStructFromDataMessage<Network::ClientData>(packet);
	//SL_LOG("%u - receving data with timestamp %f\n", index, message->getTimeStamp());

	for(int i = 0; i < _maxPlayers; i++)
	{
		if(_playerStates->clientIndex[i] == index){
			_hasSkeletonData[i] = true;
			_skeletonDatas[i] = message->_data;
			_leftHandPos[i] = message->_handPosition[VR_LEFT_HAND];
			_rightHandPos[i] = message->_handPosition[VR_RIGHT_HAND];
		}
	}
	delete message;
}
//-----------------------------------------------------------------------------
/*! 
handles the ping request
*/
void Server::sendRequestedPings()
{
	Network::PingPacket message(_timer.getElapsedTimeInMilliSec());

	RakNet::SystemAddress* systems = new RakNet::SystemAddress[_maxClients];
	unsigned short numConnections = _maxClients;
	_peer->GetConnectionList(systems, &numConnections);
	for (int i=0; i < numConnections; i++)
	{
		unsigned short index = _peer->GetIndexFromSystemAddress(systems[i]);
		if (_requestPingAnswer[index])
		{
			_requestPingAnswer[index] = false;
			SL_LOG("%u - sending ping answer\n\n", index);
			Network::SendStructMessage(_peer, systems[i], message, MEDIUM_PRIORITY, UNRELIABLE_SEQUENCED);
		}
	}

	delete[] systems;
}
//-----------------------------------------------------------------------------
/*! 
send world state to all clients
*/
void Server::sendDataToAllClients()
{
	WorldState* message = getWorldState();

	// fill message with player data
	for(int i = 0; i < _maxPlayers; i++)
	{
		if(_hasSkeletonData[i]){
			message->_playerData[i] = _skeletonDatas[i];
			message->_hasSkeletonData[i] = true;
		}else{
			message->_hasSkeletonData[i] = false;
		}
	}

	RakNet::SystemAddress* systems = new RakNet::SystemAddress[_maxClients];
	unsigned short numConnections = _maxClients;
	_peer->GetConnectionList(systems, &numConnections);
	for (int i=0; i < numConnections; i++)
	{
		unsigned short index = _peer->GetIndexFromSystemAddress(systems[i]);
		if (_sendDataToClientIndex[index] == VR_CLIENT_TYPE_OBSERVER || _sendDataToClientIndex[index] == VR_CLIENT_TYPE_PLAYER)
		{
			//SL_LOG("%u - sending data with timestamp %f\n", index, message.getTimeStamp());
			Network::SendStructMessage(_peer, systems[i], *message, MEDIUM_PRIORITY, UNRELIABLE_SEQUENCED);
		}
	}

	delete[] systems;
	delete message;
}
//-----------------------------------------------------------------------------