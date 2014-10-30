//#############################################################################
//  File:      VRNetworkClient.cpp
//  Author:    Cedric Renggli, Marc Wacker, Roman Kühne
//  Date:      Mai 2014
//#############################################################################

#include "VRNetworkClient.h"

using namespace VirtualRoom;

//-----------------------------------------------------------------------------
NetworkClient::~NetworkClient()
{
	// Try at most 500 ms to shut down the peer
	_peer->Shutdown(500, 0, MEDIUM_PRIORITY);			// USE THIS FOR PROPER SHUTDOWN ON THE CLIENTSIDE

	// Proper destroy of the RakPeer instance
	RakNet::RakPeerInterface::DestroyInstance(_peer);
}
//-----------------------------------------------------------------------------
/*! 
connect to the server
*/
void NetworkClient::connect()
{
	this->_peer = RakNet::RakPeerInterface::GetInstance();

	// Describe socket by its server port and address
	RakNet::SocketDescriptor sd;
	this->_peer->Startup(1, &sd, 1);

	const char * serverAddress = _serverAddress.c_str();
	SL_LOG("Starting the network client (connect to %s:%u)\n", serverAddress, SERVER_PORT);
	
	this->_peer->Connect(serverAddress, SERVER_PORT, 0,0);
}
//-----------------------------------------------------------------------------
/*! 
wait for the connecting answer
*/
void NetworkClient::waitForConnect()
{
	RakNet::Packet *packet;
	bool abboard = false;
	this->_connected = false;
	while (!abboard && !this->_connected)
	{
		for (packet=_peer->Receive(); packet && !abboard && !this->_connected; _peer->DeallocatePacket(packet), packet=_peer->Receive())
		{
			unsigned char packetIdentifier = Network::GetPacketIdentifier(packet);
			switch (packetIdentifier)
			{
			case ID_CONNECTION_ATTEMPT_FAILED:
				SL_LOG("Our connection request failed.\n");
				abboard = true;
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
				SL_LOG("Our connection request has been accepted.\n");
				_systemaddress = packet->systemAddress;
				this->_connected = true;
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				SL_LOG("The server is full.\n");
				abboard = true;
				break;
			default:
				SL_LOG("Message with unknown identifier %i has arrived.\n", packetIdentifier);
				break;
			}
		}
	}
}
//-----------------------------------------------------------------------------
/*! 
send the connecting question package
*/
void NetworkClient::sendConnectingQuestion()
{
	Network::ConnectingQuestion message(_timer.getElapsedTimeInMilliSec(), _clientType);
	Network::SendStructMessage(_peer, _systemaddress, message, HIGH_PRIORITY, RELIABLE);
}
//-----------------------------------------------------------------------------
/*! 
wait for the connection answer
*/
void NetworkClient::waitForConnectingAnswer(Network::ConnectingAnswer** connectingAnswer)
{
	RakNet::Packet *packet;
	// Get the start position from the server
	bool positionReceived = false;
	while(this->_connected && !positionReceived)
	{
		for (packet=_peer->Receive(); packet && this->_connected && !positionReceived; _peer->DeallocatePacket(packet), packet=_peer->Receive())
		{
			unsigned char packetIdentifier = Network::GetPacketIdentifier(packet);
			switch (packetIdentifier)
			{
			case ID_DISCONNECTION_NOTIFICATION:
				SL_LOG("We have been disconnected.\n");
				this->_connected = false;
				break;
			case ID_CONNECTION_LOST:
				SL_LOG("Connection lost.\n");
				this->_connected = false;
				break;
			case Network::ID_CONNECTING_ANSWER:
				{
					SL_LOG("Received start position from the server.\n");

					*connectingAnswer = Network::GetStructFromDataMessage<Network::ConnectingAnswer>(packet);
			
					positionReceived = true;
				}
				break;
			default:
				SL_LOG("Message with unknown identifier %i has arrived.\n", packetIdentifier);
				break;
			}
		}

		// Sleep for cpu usage
		Sleep(5);
	}
}
//-----------------------------------------------------------------------------
/*! 
handle received server data package
*/
bool NetworkClient::receiveServerData(RakNet::BitStream** bsIn)
{
	bool isDataPacket = false;
	_numberOfReceivedDataPackets = 0;

	RakNet::Packet *packet = _peer->Receive();
	RakNet::Packet *latestDataPacket = NULL;
		
	while (packet != NULL)
	{
		isDataPacket = false;

		unsigned char packetIdentifier = Network::GetPacketIdentifier(packet);
		switch (packetIdentifier)
		{
		case ID_DISCONNECTION_NOTIFICATION:
			SL_LOG("We have been disconnected.\n");
			this->_connected = false;
			break;
		case ID_CONNECTION_LOST:
			SL_LOG("Connection lost.\n");
			this->_connected = false;
			break;
		case Network::ID_DATA_SERVER:
			{
				isDataPacket = true;
				++_numberOfReceivedDataPackets;
				if (latestDataPacket != NULL)
				{
					_peer->DeallocatePacket(latestDataPacket);
				}
				latestDataPacket = packet;
			}
			break;
		case Network::ID_PING_PACKET:
			_lastPingTime = _timer.getElapsedTimeInSec() - _pingStartTime;
			_pingStartTime = -1;
			break;
		default:
			SL_LOG("Message with unknown identifier %i has arrived.\n", packetIdentifier);
			break;
		}

		// Hold last packet
		if (!isDataPacket)
		{
			_peer->DeallocatePacket(packet);
		}
		packet = _peer->Receive();
	}
	
	if(latestDataPacket != NULL)
	{
		_lastDataPacketReceived = _timer.getElapsedTimeInSec();

		*bsIn = new RakNet::BitStream(latestDataPacket->data, latestDataPacket->length, true);
		_peer->DeallocatePacket(latestDataPacket);
		return true;
	}

	return false;
}
//-----------------------------------------------------------------------------
/*! 
send current client data to the server
*/
void NetworkClient::sendClientData(SkeletonData& data, SLVec3f& leftHand, SLVec3f& rightHand)
{
	Network::ClientData message(_timer.getElapsedTimeInMilliSec());
	message._data = data;
	message._handPosition[VR_LEFT_HAND] = leftHand;
	message._handPosition[VR_RIGHT_HAND] = rightHand;
	Network::SendStructMessage(_peer, _systemaddress, message, LOW_PRIORITY, UNRELIABLE_SEQUENCED);
}
//-----------------------------------------------------------------------------
/*! 
has connection to the server
*/
bool NetworkClient::isConnected()
{
	return this->_connected;
}
//-----------------------------------------------------------------------------
/*! 
ping the server
*/
void NetworkClient::sendPingPacket()
{
	if(_pingStartTime < 0){
		Network::PingPacket message(_timer.getElapsedTimeInMilliSec());
		_pingStartTime = _timer.getElapsedTimeInSec();
		_lastPingTime = -1;
		Network::SendStructMessage(_peer, _systemaddress, message, LOW_PRIORITY, UNRELIABLE_SEQUENCED);
	}
}
//-----------------------------------------------------------------------------